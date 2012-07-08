#include "stuff.hpp"

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

void PushClassMetaTable(lua_State* state, Class& cls)
{
	// создать табличку
	lua_createtable(state, 0, cls.constructor ? 2 : 1);
	
	// если есть конструктор, сделать call
	if(cls.constructor)
	{
		lua_pushliteral(state, "__call");
		cls.constructor->PushThunk(state);
		lua_settable(state, -2);
	}

	// сделать index для статических методов
	lua_pushliteral(state, "__index");
	// создать таблицу методов
	lua_createtable(state, 0, (int)cls.staticMethods.size());
	for(size_t i = 0; i < cls.staticMethods.size(); ++i)
	{
		Function* function = cls.staticMethods[i];

		lua_pushstring(state, function->GetName());
		function->PushThunk(state);
		lua_settable(state, -2);
	}
	// таблица методов попадает в замыкание
	lua_pushcclosure(state, &MetaTable_index, 1);

	// всё.
}

void RegisterClass(lua_State* state, Class& cls)
{
	// разобрать полное имя класса (вида Inanity::Namespace1::Namespace2::ClassName),
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
		for(j = i + 1; j < fullName.length() && fullName[j] != ':'; ++j);
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

		// передвинуть i, чтобы пройти второе двоеточие
		for(i = j; i < fullName.length() && fullName[i] == ':'; ++i);
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
	lua_pushlightuserdata(state, &cls);
	lua_gettable(state, LUA_REGISTRYINDEX);
	// если её нет, сделать
	if(!lua_istable(state, -1))
	{
		// выбросить nil из стека
		lua_pop(state, 1);

		// таблица методов нужна только в __index.
		// так пусть она будет у __index в замыкании :)
		// и никуда больше её не сохранять, здорово ^_^

		// создать метатаблицу
		// использовать адрес структуры класса как индекс в реестре
		lua_pushlightuserdata(state, &cls);
		lua_createtable(state, 0, 2);
		// задать индексатор
		lua_pushliteral(state, "__index");

		// вот в этом месте создать таблицу методов
		lua_pushlightuserdata(state, &cls.methods);
		lua_createtable(state, 0, (int)cls.methods.size());
		for(size_t i = 0; i < cls.methods.size(); ++i)
		{
			Method* method = cls.methods[i];
			lua_pushstring(state, method->GetName());
			method->PushThunk(state);
			lua_settable(state, -3);
		}
		// и засунуть её в замыкание
		lua_pushcclosure(state, &MetaTable_index, 1);
		lua_settable(state, -3);

		// задать деструктор
		lua_pushliteral(state, "__gc");
		lua_pushcclosure(state, &ObjectMetaTable_gc, 0);
		lua_settable(state, -3);

		// и положить метатаблицу в реестр
		lua_settable(state, LUA_REGISTRYINDEX);
	}
}

END_INANITY_LUA
