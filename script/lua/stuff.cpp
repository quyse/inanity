#include "stuff.hpp"
#include "values.ipp"
#include "MetaProvider.ipp"
#include "../../Exception.hpp"
#include <sstream>

BEGIN_INANITY_LUA

int MetaTable_index(lua_State* state)
{
	// в стеке лежит: сначала userdata, затем индекс
	// а в замыкании лежит таблица методов

	// проверить, что объект не отозван
	ObjectUserData* userData = (ObjectUserData*)lua_touserdata(state, -2);
	if(!userData->object)
	{
		// вернуть понятную ошибку
		lua_pushliteral(state, "Object reclaimed");
		lua_error(state);
		// управление не возвращается
	}

	// продублировать индекс
	lua_pushvalue(state, 2);
	// получить метод
	lua_gettable(state, lua_upvalueindex(1));

	// если такого метода нет
	if(lua_isnil(state, -1))
	{
		// вернуть понятную ошибку
		lua_pushliteral(state, "No such method: ");
		lua_pushvalue(state, 2);
		lua_concat(state, 2);
		lua_error(state);
	}

	return 1;
}

/// Функция для сообщения об ошибке - попытке создать объект класса без конструктора.
int NonConstructableClass_call(lua_State* state)
{
	// в стеке лежит: сначала userdata, затем параметры какие-то
	// а в замыкании лежит полное имя класса

	// выбросить исключение
	lua_pushliteral(state, "Trying to create object of non-constructable class ");
	lua_pushvalue(state, lua_upvalueindex(1));
	lua_concat(state, 2);
	lua_error(state);
	return 0;
}

void PushClassMetaTable(lua_State* state, MetaProvider::ClassBase* cls)
{
	// создать табличку
	lua_createtable(state, 0, 2);     // metatable
	
	// сделать call
	lua_pushliteral(state, "__call");                    // metatable "__call"
	MetaProvider::ConstructorBase* constructor = cls->GetConstructor();
	// если конструктор есть, то добавить его
	if(constructor)
		constructor->PushThunk(state);                     // metatable "__call" thunk
	else
	{
		// а если конструктора нет, добавить заглушку
		lua_pushstring(state, cls->GetFullName());         // metatable "__call" classFullName
		lua_pushcclosure(state, NonConstructableClass_call, 1); // metatable "__call" thunk
	}
	lua_settable(state, -3);                             // metatable

	// сделать index для статических методов
	lua_pushliteral(state, "__index");                   // metatable "__index"
	// создать таблицу методов
	const MetaProvider::ClassBase::StaticMethods& staticMethods = cls->GetStaticMethods();
	lua_createtable(state, 0, (int)staticMethods.size()); // metatable "__index" staticMethods
	for(size_t i = 0; i < staticMethods.size(); ++i)
	{
		MetaProvider::FunctionBase* function = staticMethods[i];

		lua_pushstring(state, function->GetName());        // metatable "__index" staticMethods functionName
		function->PushThunk(state);                        // metatable "__index" staticMethods functionName thunk
		lua_settable(state, -3);                           // metatable "__index" staticMethods
	}
	// таблица методов попадает в замыкание
	lua_pushcclosure(state, &MetaTable_index, 1);        // metatable "__index" __index
	// положить обработчик в таблицу
	lua_settable(state, -3);                             // metatable

	// всё.
}

void RegisterClassMeta(lua_State* state, MetaProvider::ClassBase* cls)
{
	// разобрать полное имя класса (вида Inanity.Namespace1.Namespace2.ClassName),
	// пройтись по этому пути, и создать глобальный объект
	const char* fullName = cls->GetFullName();
	// поддерживаем инвариант - в стеке лежит последний объект пройденного пути
	// сначала тут, конечно, глобальное пространство
	lua_pushinteger(state, LUA_RIDX_GLOBALS);
	lua_gettable(state, LUA_REGISTRYINDEX);
	// цикл по кускам пути
	for(size_t i = 0; fullName[i]; )
	{
		size_t j;
		for(j = i + 1; fullName[j] && fullName[j] != '.'; ++j);
		// тут [i, j) задают строку пути
		// положить строку в стек
		lua_pushlstring(state, fullName + i, j - i);             // prevPath name

		// если это последняя строка
		if(!fullName[j])
		{
			// создать объект
			ClassUserData* userData = (ClassUserData*)lua_newuserdata(state, sizeof(ClassUserData)); // prevPath name userData
			(void)userData;
			// указать ему метатаблицу
			PushClassMetaTable(state, cls);                        // prevPath name userData metatable
			lua_setmetatable(state, -2);                           // prevPath name userData
			// и добавить его в таблицу
			lua_settable(state, -3);                               // prevPath

			// завершить цикл
			break;
		}
		// иначе это промежуточная строка
		else
		{
			// сразу продублировать строку, на случай, если опять понадобится
			lua_pushvalue(state, -1);                              // prevPath name fullName
			lua_gettable(state, -3);                               // prevPath name curPath
			// если получена
			if(lua_istable(state, -1))
			{
				// выбросить копию строки и предыдущий путь
				lua_replace(state, -3);                              // curPath name
				lua_pop(state, 1);                                   // curPath
			}
			// иначе нету
			else
			{
				// выбросить nil
				lua_pop(state, 1);                                   // prevPath name
				// создать таблицу
				lua_newtable(state);                                 // prevPath name curPath
				// продублировать её перед предыдущим путём
				lua_pushvalue(state, -1);                            // prevPath name curPath curPath
				lua_insert(state, lua_absindex(state, -4));          // curPath prevPath name curPath
				// положить её в предыдущую таблицу
				lua_settable(state, -3);                             // curPath prevPath
				// выкинуть предыдущую таблицу
				lua_pop(state, 1);
			}
		}

		// передвинуть i, чтобы пройти точку
		i = j + 1;
	}

	// очистить стек
	lua_pop(state, 1);
}

int ObjectMetaTable_gc(lua_State* state)
{
	// в стеке лежит: userdata

	ReclaimObjectFromUserData(state);

	return 0;
}

void ReclaimObjectFromUserData(lua_State* state)
{
	ObjectUserData* userData = (ObjectUserData*)lua_touserdata(state, -1);
	// if object is not reclaimed yet, reclaim it
	RefCounted* object = userData->object;
	if(object)
	{
		// dereference object
		userData->object->Dereference();
		// clear pointer
		userData->object = 0;

		// remove userdata from registry
		lua_pushlightuserdata(state, object);
		lua_pushnil(state);
		lua_settable(state, LUA_REGISTRYINDEX);
	}
}

void ReclaimObject(lua_State* state, RefCounted* object)
{
	// find an object by pointer
	lua_pushlightuserdata(state, object);
	lua_gettable(state, LUA_REGISTRYINDEX);
	// if object doesn't exist, just pop nil from stack
	if(lua_isnil(state, -1))
		lua_pop(state, 1);
	// else reclaim the object
	else
		ReclaimObjectFromUserData(state);
}

void PushObjectMetaTable(lua_State* state, MetaProvider::ClassBase* cls)
{
	// получить метатаблицу объектов
	lua_pushlightuserdata(state, cls);                   // cls
	lua_gettable(state, LUA_REGISTRYINDEX);              // metatable
	// если её нет, сделать
	if(!lua_istable(state, -1))
	{
		// выбросить nil из стека
		lua_pop(state, 1);                                 //

		// таблица методов нужна только в __index.
		// так пусть она будет у __index в замыкании :)
		// и никуда больше её не сохранять, здорово ^_^

		// создать метатаблицу
		// использовать адрес структуры класса как индекс в реестре
		lua_pushlightuserdata(state, cls);                 // cls
		lua_createtable(state, 0, 2);                      // cls metatable
		// задать индексатор
		lua_pushliteral(state, "__index");                 // cls metatable "__index"

		// вот в этом месте создать таблицу методов
		// посчитать количество методов
		size_t methodsCount = 0;
		for(MetaProvider::ClassBase* c = cls; c; c = c->GetParent())
			methodsCount += c->GetMethods().size();
		// создать таблицу
		lua_createtable(state, 0, (int)methodsCount);      // cls metatable "__index" methods
		// и наполнить методами
		for(MetaProvider::ClassBase* c = cls; c; c = c->GetParent())
		{
			const MetaProvider::ClassBase::Methods& methods = c->GetMethods();
			for(size_t i = 0; i < methods.size(); ++i)
			{
				MetaProvider::MethodBase* method = methods[i];
				lua_pushstring(state, method->GetName());      // cls metatable "__index" methods methodName
				method->PushThunk(state);                      // cls metatable "__index" methods methodName thunk
				lua_settable(state, -3);                       // cls metatable "__index" methods
			}
		}
		// и засунуть её в замыкание
		lua_pushcclosure(state, &MetaTable_index, 1);      // cls metatable "__index" MetaTable_index
		lua_settable(state, -3);                           // cls metatable

		// задать деструктор
		lua_pushliteral(state, "__gc");                    // cls metatable "__gc"
		lua_pushcclosure(state, &ObjectMetaTable_gc, 0);   // cls metatable "__gc" ObjectMetaTable_gc
		lua_settable(state, -3);                           // cls metatable

		// немного попереставлять, чтобы потом осталась метатаблица
		lua_pushvalue(state, -1);                          // cls metatable metatable
		lua_insert(state, lua_absindex(state, -3));        // metatable cls metatable
		// и положить метатаблицу в реестр
		lua_settable(state, LUA_REGISTRYINDEX);            // metatable
	}
}

ptr<Exception> ErrorToException(lua_State* state)
{
	ptr<Exception> exception;
	// посмотреть, не ptr<Exception> ли
	if(lua_isuserdata(state, -1) && !lua_islightuserdata(state, -1))
	{
		// full userdata, возможно, что Exception
		ObjectUserData* userData = (ObjectUserData*)lua_touserdata(state, -1);
		if(userData->type == UserData::typeObject && userData->cls == Meta::MetaOf<MetaProvider, Exception>())
			// да, Exception!
			exception = (Exception*)userData->object;
	}
	// если это не Exception, то вывести, как произвольное значение
	if(!exception)
	{
		std::ostringstream stream;
		stream << "Lua error: ";
		DescribeValue(state, -1, stream);
		exception = NEW(Exception(stream.str()));
	}

	lua_pop(state, 1);

	return exception;
}

void ProcessError(lua_State* state)
{
	THROW_SECONDARY("Error while running Lua function", ErrorToException(state));
}

int ScriptErrorHook(lua_State* state)
{
	// в стеке: сообщение об ошибке

	// собрать информацию о стеке вызовов
	std::ostringstream stream;
	stream << "Lua error backtrace:\n";
	lua_Debug ar;
	for(int i = 0; lua_getstack(state, i, &ar); ++i)
	{
		// получить активационную запись, и положить в стек Lua функцию на этом уровне
		if(!lua_getinfo(state, "nSltuf", &ar))
		{
			// по идее, ошибки быть не должно
			stream << "<error while getting activation record>\n";
			break;
		}

		// имя файла и номер строки
		stream << ar.short_src << ':';
		if(ar.currentline >= 0)
			stream << ar.currentline;
		else
			stream << "??";
		stream << ' ';
		// тип функции
		if(ar.what)
			stream << ar.what;
		// имя функции
		stream << ' ' << (ar.name ? ar.name : "<function>");
		// значения в замыкании (upvalues), если есть
		if(ar.nups)
		{
			stream << '[';
			for(int j = 1; j <= ar.nups; ++j)
			{
				// положить значение upvalue в стек, и получить имя upvalue
				const char* upvalueName = lua_getupvalue(state, -1, j);

				if(j > 1)
					stream << ", ";

				// вывести имя upvalue
				if(upvalueName && upvalueName[0])
					stream << upvalueName << " = ";
				// вывести значение upvalue
				DescribeValue(state, -1, stream);
				// выбросить значение из стека
				lua_pop(state, 1);
			}
			stream << ']';
		}
		// параметры
		stream << '(';
		// сначала фиксированные параметры
		for(int j = 1; j <= ar.nparams; ++j)
		{
			// положить значение параметра в стек, и получить его имя
			const char* paramName = lua_getlocal(state, &ar, j);

			if(j > 1)
				stream << ", ";

			// вывести имя параметра
			if(paramName && paramName[0])
				stream << paramName << " = ";
			// вывести значение параметра
			DescribeValue(state, -1, stream);
			// выбросить значение из стека
			lua_pop(state, 1);
		}
		// теперь, если предполагаются vararg-параметры
		if(ar.isvararg)
		{
			stream << " ... ";
			// получить и их
			const char* paramName;
			for(int j = -1; (paramName = lua_getlocal(state, &ar, j)); --j)
			{
				if(j < -1)
					stream << ", ";

				// вывести имя параметра
				if(paramName && paramName[0])
					stream << paramName << " = ";
				// вывести значение параметра
				DescribeValue(state, -1, stream);
				// выбросить значение из стека
				lua_pop(state, 1);
			}
		}
		// и теперь ещё попробуем вывести локальные переменные и временные значения
		{
			const char* paramName;
			for(int j = ar.nparams + 1; (paramName = lua_getlocal(state, &ar, j)); ++j)
			{
				stream << (j > ar.nparams + 1 ? ", " : " | ");

				// вывести имя параметра
				if(paramName && paramName[0])
					stream << paramName << " = ";
				// вывести значение параметра
				DescribeValue(state, -1, stream);
				// выбросить значение из стека
				lua_pop(state, 1);
			}
		}
		stream << ')';

		// если это хвостовой вызов, отметить это
		if(ar.istailcall)
			stream << " [tailcall]";

		stream << '\n';

		// выбросить функцию из стека
		lua_pop(state, 1);
	}

	// положить в стек исключение
	Value<ptr<Exception> >::Push(state, NEW(Exception(stream.str(), ErrorToException(state))));
	return 1;
}

void DescribeValue(lua_State* state, int index, std::ostream& stream)
{
	switch(lua_type(state, index))
	{
	case LUA_TNONE:
		stream << "<non-valid index>";
		break;
	case LUA_TNIL:
		stream << "nil";
		break;
	case LUA_TNUMBER:
		stream << lua_tonumber(state, index);
		break;
	case LUA_TBOOLEAN:
		stream << (lua_toboolean(state, index) ? "true" : "false");
		break;
	case LUA_TSTRING:
		stream << '"' << lua_tostring(state, index) << '"';
		break;
	case LUA_TTABLE:
		{
			stream << "{ ";
			lua_checkstack(state, 2);
			index = lua_absindex(state, index);
			lua_pushnil(state);             // table ... nil
			bool notFirst = false;
			while(lua_next(state, index))
			{
				                              // table ... key value
				if(notFirst)
					stream << ", ";
				else
					notFirst = true;
				stream << "[ ";
				DescribeValue(state, -2, stream);
				stream << " ] = ";
				DescribeValue(state, -1, stream);

				lua_pop(state, 1);            // table ... key
			}
			stream << " }";
		}
		break;
	case LUA_TFUNCTION:
		stream << "<function>";
		break;
	case LUA_TUSERDATA:
		{
			UserData* userData = (UserData*)lua_touserdata(state, index);
			switch(userData->type)
			{
			case UserData::typeClass:
				// TODO: разобрать, что это
				stream << "<ClassUserData>";
				break;
			case UserData::typeObject:
				{
					ObjectUserData* objectUserData = (ObjectUserData*)userData;
					stream << "ptr<" << objectUserData->cls->GetFullName() << ">";
				}
				break;
			default:
				stream << "<unknown full userdata>";
				break;
			}
		}
		break;
	case LUA_TTHREAD:
		stream << "<thread>";
		break;
	case LUA_TLIGHTUSERDATA:
		stream << "<light userdata: " << lua_touserdata(state, index) << ">";
		break;
	default:
		stream << "<unknown value>";
		break;
	}
}

END_INANITY_LUA
