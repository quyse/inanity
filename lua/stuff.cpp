#include "stuff.hpp"
#include "reflection.hpp"
#include "values.hpp"

BEGIN_INANITY_LUA

int MetaTable_index(lua_State* state)
{
	// в стеке лежит: сначала userdata, затем индекс
	// а в замыкании лежит таблица методов

	// получить метод
	lua_gettable(state, lua_upvalueindex(1));
	// проверять, что он есть, не нужно
	// если что, будет nil, так что пофиг
	return 1;
}

/// Функция для сообщения об ошибке - попытке создать объект класса без конструктора.
int NonConstructableClass_call(lua_State* state)
{
	// в стеке лежит: сначала userdata, затем параметры какие-то
	// а в замыкании лежит полное имя класса

	// выбросить исключение
	Value<ptr<Exception> >::Push(state, NEW(Exception(String("Trying to create object of non-constructable class ") + lua_tostring(state, lua_upvalueindex(1)))));
	lua_error(state);
	return 0;
}

void PushClassMetaTable(lua_State* state, Class& cls)
{
	// создать табличку
	lua_createtable(state, 0, 2);     // metatable
	
	// сделать call
	lua_pushliteral(state, "__call");                    // metatable "__call"
	Constructor* constructor = cls.GetConstructor();
	// если конструктор есть, то добавить его
	if(constructor)
		constructor->PushThunk(state);                     // metatable "__call" thunk
	else
	{
		// а если конструктора нет, добавить заглушку
		lua_pushstring(state, cls.GetFullName().c_str());  // metatable "__call" classFullName
		lua_pushcclosure(state, NonConstructableClass_call, 1); // metatable "__call" thunk
	}
	lua_settable(state, -3);                             // metatable

	// сделать index для статических методов
	lua_pushliteral(state, "__index");                   // metatable "__index"
	// создать таблицу методов
	const std::vector<Function*>& staticMethods = cls.GetStaticMethods();
	lua_createtable(state, 0, (int)staticMethods.size()); // metatable "__index" staticMethods
	for(size_t i = 0; i < staticMethods.size(); ++i)
	{
		Function* function = staticMethods[i];

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

void RegisterClass(lua_State* state, Class& cls)
{
	// разобрать полное имя класса (вида Inanity.Namespace1.Namespace2.ClassName),
	// пройтись по этому пути, и создать глобальный объект
	String fullName = cls.GetFullName();
	// поддерживаем инвариант - в стеке лежит последний объект пройденного пути
	// сначала тут, конечно, глобальное пространство
	lua_pushinteger(state, LUA_RIDX_GLOBALS);
	lua_gettable(state, LUA_REGISTRYINDEX);
	// цикл по кускам пути
	for(size_t i = 0; i < fullName.length(); )
	{
		size_t j;
		for(j = i + 1; j < fullName.length() && fullName[j] != '.'; ++j);
		// тут [i, j) задают строку пути
		// положить строку в стек
		lua_pushlstring(state, fullName.c_str() + i, j - i);   // prevPath name

		// если это последняя строка
		if(j >= fullName.length())
		{
			// создать объект
			ClassUserData* userData = (ClassUserData*)lua_newuserdata(state, sizeof(ClassUserData)); // prevPath name userData
			(void)userData;
			// указать ему метатаблицу
			PushClassMetaTable(state, cls);                        // prevPath name userData metatable
			lua_setmetatable(state, -2);                           // prevPath name userData
			// и добавить его в таблицу
			lua_settable(state, -3);                               // prevPath
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

	ObjectUserData* userData = (ObjectUserData*)lua_touserdata(state, -1);
	// освободить ссылку на объект
	userData->object->Dereference();
	userData->object = 0;
	userData->cls = 0;
	return 0;
}

void PushObjectMetaTable(lua_State* state, Class& cls)
{
	// получить метатаблицу объектов
	lua_pushlightuserdata(state, &cls);                  // cls
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
		lua_pushlightuserdata(state, &cls);                // cls
		lua_createtable(state, 0, 2);                      // cls metatable
		// задать индексатор
		lua_pushliteral(state, "__index");                 // cls metatable "__index"

		// вот в этом месте создать таблицу методов
		// посчитать количество методов
		size_t methodsCount = 0;
		for(Class* c = &cls; c; c = c->GetParent())
			methodsCount += c->GetMethods().size();
		// создать таблицу
		lua_createtable(state, 0, (int)methodsCount);      // cls metatable "__index" methods
		// и наполнить методами
		for(Class* c = &cls; c; c = c->GetParent())
		{
			const std::vector<Method*>& methods = c->GetMethods();
			for(size_t i = 0; i < methods.size(); ++i)
			{
				Method* method = methods[i];
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

void ProcessError(lua_State* state)
{
	// посмотреть, не ptr<Exception> ли
	if(lua_isuserdata(state, -1) && !lua_islightuserdata(state, -1))
	{
		// full userdata, возможно, что Exception
		ObjectUserData* userData = (ObjectUserData*)lua_touserdata(state, -1);
		if(userData->type == UserData::typeObject && userData->cls == &Exception::scriptClass)
			// да, Exception! завернуть его в новое исключение
			THROW_SECONDARY_EXCEPTION("Exception while running Lua function", (Exception*)userData->object);
	}
	// это не Exception, хз, чё это
	// если строка
	const char* errorString = lua_tostring(state, -1);
	if(errorString)
		THROW_PRIMARY_EXCEPTION(String("Error while running Lua function: ") + errorString);
	// иначе совсем хз
	THROW_PRIMARY_EXCEPTION("Unknown error while running Lua function");
}

String DescribeValue(lua_State* state, int index)
{
	switch(lua_type(state, index))
	{
	case LUA_TNONE:
		return "<non-valid index>";
	case LUA_TNIL:
		return "nil";
	case LUA_TNUMBER:
		{
			char str[10];
			sprintf(str, "%lf", lua_tonumber(state, index));
			return str;
		}
	case LUA_TBOOLEAN:
		return lua_toboolean(state, index) ? "true" : "false";
	case LUA_TSTRING:
		return String("\"") + lua_tostring(state, index) + "\"";
	case LUA_TTABLE:
		// TODO: сделать разбор таблицы
		return "<table>";
	case LUA_TFUNCTION:
		return "<function>";
	case LUA_TUSERDATA:
		{
			UserData* userData = (UserData*)lua_touserdata(state, index);
			switch(userData->type)
			{
			case UserData::typeClass:
				// TODO: разобрать, что это
				return "<ClassUserData>";
			case UserData::typeObject:
				{
					ObjectUserData* objectUserData = (ObjectUserData*)userData;
					return "ptr<" + objectUserData->cls->GetFullName() + ">";
				}
			}
			return "<unknown full userdata>";
		}
	case LUA_TTHREAD:
		return "<thread>";
	case LUA_TLIGHTUSERDATA:
		return "<light userdata>";
	}
	return "<unknown value>";
}

END_INANITY_LUA
