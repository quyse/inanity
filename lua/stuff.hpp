#ifndef ___INANITY_LUA_CALLING_HPP___
#define ___INANITY_LUA_CALLING_HPP___

/* Файл содержит различные шаблонные функции.
Часто функции оформляются как статические методы в структурах,
только потому, что функции не могут иметь частичных специализаций.
*/

#include "lua.hpp"
#include "lualib.hpp"
#include "State.hpp"
#include "userdata.hpp"
#include "reflection.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_LUA

/// Универсальная функция для индексирования метатаблиц (классов и объектов).
/** Предполагает, что в замыкании лежит таблица методов. */
int MetaTable_index(lua_State* state)
{
	// в стеке лежит: сначала userdata, затем индекс
	// а в замыкании лежит таблица методов

	// получить метод
	lus_gettable(state, lua_upvalueindex(1));
	// проверять, что он есть, не нужно
	// если что, будет nil, так что пофиг
	return 1;
}

/// Запихать в стек метатаблицу для классов.
/** Метатаблица классов нигде не хранится, и создаётся каждый раз заново.
Потому что предполагается, что она один раз будет нужна (для одного глобального объекта).
Вообще говоря, можно было бы обойтись вообще без метатаблицы, а сделать просто таблицу,
а не объект. Но пофиг, пусть будет более круто. */
void PushClassMetaTable(lua_State* state, const Class& cls)
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
		const Function* function = cls.staticMethods[i];

		lua_pushstring(state, function->GetName());
		function->PushThunk(state);
		lua_settable(state, -2);
	}
	// таблица методов попадает в замыкание
	lua_pushcclosure(state, &MetaTable_index, 1);

	// всё.
}

/// Зарегистрировать класс в состоянии.
void RegisterClass(lua_State* state, const Class& cls)
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
				lua_createtable(state);                              // prevPath name curPath
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

/// Структура с методами метатаблицы объектов.
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

/// Запихать в стек метатаблицу для объектов.
void PushObjectMetaTable(lua_State* state, const Class& cls)
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
			const Method* method = cls.methods[i];
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

/// Структура, сохраняющая состояние для получения аргументов.
struct ArgGettingState
{
	/// Состояние Lua.
	lua_State* state;
	/// Общее количество аргументов.
	int argsCount;
	/// Количество уже вытащенных аргументов.
	int gotArgsCount;
};

/// Вспомогательная функция для получения аргумента из стека Lua.
/** Использует структуру для получения кучи аргументов, и увеличивает счётчик. */
template <typename ArgType>
struct NextArgGetter
{
	static inline ArgType Get(ArgGettingState* state)
	{
		return ArgGetter<ArgType>::Get(state->state, ++state->gotArgsCount);
	}
};
// для void отдельная специализация, не увеличивающая счётчик
template <>
struct NextArgGetter<void>
{
	static inline void Get(ArgGettingState* state) {}
};

/// Структура для получения аргумента из стека.
template <typename ArgType>
struct ArgGetter;
template <>
struct ArgGetter<void>
{
	static inline void Get(lua_State* state, int index) {}
};
template <>
struct ArgGetter<bool>
{
	static inline bool Get(lua_State* state, int index)
	{
		return lua_toboolean(state, index);
	}
};
template <>
struct ArgGetter<int>
{
	static inline int Get(lua_State* state, int index)
	{
		int isnum;
		int res = lua_tointegerx(state, index, &isnum);
		if(!isnum)
			THROW_PRIMARY_EXCEPTION("Expected an integer for argument");
		return res;
	}
};
template <>
struct ArgGetter<const char*>
{
	static inline const char* Get(lua_State* state, int index)
	{
		const char* res = lua_tostring(state, index);
		if(!res)
			THROW_PRIMARY_EXCEPTION("Expected a string for argument");
		return res;
	}
};
template <>
struct ArgGetter<String>
{
	static inline String Get(lua_State* state, int index)
	{
		return ArgGetter<const char*>::Get(state, index);
	}
};
template <typename ObjectType>
struct ArgGetter<ptr<ObjectType> >
{
	static inline ptr<ObjectType> Get(lua_State* state, int index)
	{
		// получить userdata для объекта, и проверить, что это объект
		UserDataObject* userData = (UserDataObject*)lua_touserdata(state, index);
		if(!userData || lua_islightuserdata(state, index) || userData->type != UserData::typeObject)
			THROW_PRIMARY_EXCEPTION("Expected an object for argument");
		// проверить тип объекта
		if(&ObjectType::scriptClass != userData->cls)
			THROW_PRIMARY_EXCEPTION("An object of another type was expected for argument");
		// вернуть объект
		return (ObjectType*)userData->object;
	}
};

/// Вспомогательная структура для запихивания аргумента в стек Lua.
template <typename ArgType>
struct ArgPusher;
template <>
struct ArgPusher<void>
{
	static void Push(lua_State* state, void)
	{
		return 0;
	}
};
template <>
struct ArgPusher<bool>
{
	static int Push(lua_State* state, bool arg)
	{
		lua_pushboolean(state, (int)arg);
		return 1;
	}
};
template <>
struct ArgPusher<int>
{
	static int Push(lua_State* state, int arg)
	{
		lua_pushinteger(state, arg);
		return 1;
	}
};
template <>
struct ArgPusher<const char*>
{
	static void Push(lua_State* state, const char* arg)
	{
		lua_pushstring(state, arg);
	}
};
template <>
struct ArgPusher<String>
{
	static void Push(lua_State* state, const String& arg)
	{
		lua_pushstring(state, arg.c_str());
	}
};
template <typename ObjectType>
struct ArgPusher<ptr<ObjectType> >
{
	static void Push(lua_State* state, ptr<ObjectType> arg)
	{
		PushObjectMetaTable<ObjectType>(state);
		ObjectUserData* userData = (ObjectUserData*)lua_newuserdata(state, sizeof(ObjectUserData));
		userData->type = UserData::typeObject;
		userData->object = (Object*)(ObjectType*)arg;
		userData->class = &ObjectType::scriptClass;
		// задать дополнительную ссылку объекту
		userData->object->Reference();
	}
};

/// Вспомогательная структура для запихивания кучи аргументов в стек.
template <typename... ArgTypes>
struct ArgsPusher;
// Когда нет аргументов.
template <>
struct ArgsPusher<>
{
	inline static void Push(lua_State* state) {}
};
// И когда есть хотя бы один аргумент.
template <typename FirstArgType, typename... RestArgTypes>
struct ArgsPusher<FirstArgType, RestArgTypes...>
{
	inline static void Push(lua_State* state, FirstArgType firstArg, RestArgTypes... restArgs)
	{
		// запихать первый аргумент
		ArgPusher<FirstArgType>::Push(state, firstArg);
		// и остальные
		ArgsPusher<RestArgTypes...>::Push(state, restArgs...);
	}
};

/// Структура-контейнер списка типов.
/** Только объявление шаблона, определение не требуется. */
template <typename... ArgTypes>
struct Args;

/// Вспомогательная структура для вызова функции.
template <typename FunctionType, FunctionType function, typename GotArgs, typename RestArgs>
struct FunctionCaller;
// Когда все аргументы уже получены.
template <typename ReturnType, typename... ArgTypes, ReturnType (*function)(ArgTypes...)>
struct FunctionCaller<ReturnType (*)(ArgTypes...), function, Args<ArgTypes...>, Args<> >
{
	inline static ReturnType Call(ArgGettingState* state, ArgTypes... args)
	{
		// в стеке не должно остаться аргументов
		if(state->gotArgsCount != state->argsCount)
			THROW_PRIMARY_EXCEPTION("Extra arguments for method call");
		// всё путём, вызвать функцию
		return function(args...);
	}
};
// И когда хотя бы один аргумент остался.
template <typename ReturnType, typename... GotArgTypes, typename FirstRestArgType, typename... MoreRestArgTypes, ReturnType (*function)(GotArgTypes..., FirstRestArgType, MoreRestArgTypes...)>
struct FunctionCaller<ReturnType (*)(GotArgTypes..., FirstRestArgType, MoreRestArgTypes...), function, Args<GotArgTypes...>, Args<FirstRestArgType, MoreRestArgTypes...> >
{
	inline static ReturnType Call(ArgGettingState* state, GotArgTypes... gotArgs)
	{
		// в стеке должен быть аргумент
		if(state->gotArgsCount >= state->argsCount)
			THROW_PRIMARY_EXCEPTION("Not enough arguments for method call");
		// аргумент есть, получить его и двигаться дальше
		return Caller<ReturnType (*)(GotArgTypes..., FirstRestArgType, MoreRestArgTypes...), function, Args<GotArgTypes..., FirstRestArgType>, Args<MoreRestArgTypes...> >::Call(gotArgs..., NextArgGetter<FirstRestArgType>::Get(state));
	}
};

/// Вспомогательная структура для вызова метода.
template <typename MethodType, MethodType method, typename GotArgs, typename RestArgs>
struct MethodCaller;
// Когда все аргументы уже получены.
template <typename ReturnType, typename ClassType, typename... ArgTypes, ReturnType (ClassType::*method)(ArgTypes...)>
struct MethodCaller<ReturnType (ClassType::*)(ArgTypes...), method, Args<ArgTypes...>, Args<> >
{
	inline static ReturnType Call(ArgGettingState* state, ClassType* object, ArgTypes... args)
	{
		// в стеке не должно остаться аргументов
		if(state->gotArgsCount != state->argsCount)
			THROW_PRIMARY_EXCEPTION("Extra arguments for method call");
		// всё путём, вызвать метод
		return (object->*method)(args...);
	}
};
// И когда хотя бы один аргумент остался.
template <typename ReturnType, typename ClassType, typename... GotArgTypes, typename FirstRestArgType, typename... MoreRestArgTypes, ReturnType (ClassType::*method)(GotArgTypes..., FirstRestArgType, MoreRestArgTypes...)>
struct MethodCaller<ReturnType (ClassType::*)(GotArgTypes..., FirstRestArgType, MoreRestArgTypes...), method, Args<GotArgTypes...>, Args<FirstRestArgType, MoreRestArgTypes...> >
{
	inline static ReturnType Call(ArgGettingState* state, ClassType* object, GotArgTypes... gotArgs)
	{
		// в стеке должен быть аргумент
		if(state->gotArgsCount >= state->argsCount)
			THROW_PRIMARY_EXCEPTION("Not enough arguments for method call");
		// аргумент есть, получить его и двигаться дальше
		return MethodCaller<ReturnType (ClassType::*)(GotArgTypes..., FirstRestArgType, MoreRestArgTypes...), method, Args<GotArgTypes..., FirstRestArgType>, Args<MoreRestArgTypes...> >::Call(object, gotArgs..., NextArgGetter<FirstRestArgType>::Get(state));
	}
};

/// Вспомогательная структура для вызова конструктора.
template <typename ClassType, typename GotArgs, typename RestArgs>
struct ConstructorCaller;
// Когда все аргументы уже получены.
template <typename ClassType, typename... ArgTypes>
struct ConstructorCaller<ClassType, Args<ArgTypes...>, Args<> >
{
	inline static ClassType* Call(ArgGettingState* state, ArgTypes... args)
	{
		// в стеке не должно остаться аргументов
		if(state->gotArgsCount != state->argsCount)
			THROW_PRIMARY_EXCEPTION("Extra arguments for method call");
		// всё путём, создать объект
		return NEW(ClassType(args...));
	}
};
// И когда хотя бы один аргумент остался.
template <typename ClassType, typename... GotArgTypes, typename FirstRestArgType, typename... MoreRestArgTypes>
struct ConstructorCaller<ClassType, Args<GotArgTypes...>, Args<FirstRestArgType, MoreRestArgTypes...> >
{
	inline static ClassType* Call(ArgGettingState* state, GotArgTypes... gotArgs)
	{
		// в стеке должен быть аргумент
		if(state->gotArgsCount >= state->argsCount)
			THROW_PRIMARY_EXCEPTION("Not enough arguments for method call");
		// аргумент есть, получить его и двигаться дальше
		return ConstructorCaller<ClassType, Args<GotArgTypes..., FirstRestArgType>, Args<MoreRestArgTypes...> >::Call(gotArgs..., NextArgGetter<FirstRestArgType>::Get(state));
	}
};

/// Структура превращения функций в функции Lua.
template <typename FunctionType, FunctionType function>
struct FunctionThunk;
// Специализация, разбирающая тип функции.
template <typename ReturnType, typename... ArgTypes, ReturnType (*function)(ArgTypes...)>
struct FunctionThunk<ReturnType (*)(ArgTypes...), function>
{
	/// Функция, вызывающая метод у объекта.
	/** Следует формату и протоколу lua_CFunction. */
	static int Thunk(lua_State* luaState)
	{
		try
		{
			// сформировать состояние для получения аргументов
			ArgGettingState state;
			state.state = luaState;
			// получить действительное количество аргументов
			state.argsCount = lua_gettop(luaState);
			state.gotArgsCount = 0;

			// получить аргументы, выполнить вызов и положить результат в стек
			// возвращается количество результатов
			return ArgPusher<ReturnType>::Push(luaState, FunctionCaller<ReturnType (*)(ArgTypes...), function, Args<>, Args<ArgTypes...> >::Call(&state));
		}
		catch(Exception* exception)
		{
			// положить в стек исключение
			State::Push(state, MakePointer(exception));
			// и выбросить ошибку
			lua_error(state);
		}
	}
};

/// Структура превращения методов в функции-методы lua.
template <typename MethodType, MethodType method>
struct MethodThunk;
// Специализация, разбирающая тип метода.
template <typename ReturnType, typename ClassType, typename... ArgTypes, ReturnType (ClassType::*method)(ArgTypes...)>
struct MethodThunk<ReturnType (ClassType::*)(ArgTypes...), method>
{
	/// Функция, вызывающая метод у объекта.
	/** Следует формату и протоколу lua_CFunction. */
	static int Thunk(lua_State* luaState)
	{
		try
		{
			// сформировать состояние для получения аргументов
			ArgGettingState state;
			state.state = luaState;
			// получить действительное количество аргументов
			state.argsCount = lua_gettop(luaState);
			state.gotArgsCount = 0;

			// получить объект this
			ptr<ClassType> thisObject = NextArgGetter<ptr<ClassType> >::Get(&state);

			// получить аргументы, выполнить вызов и положить результат в стек
			// возвращается количество результатов
			return ArgPusher<ReturnType>::Push(luaState, MethodCaller<ReturnType (ClassType::*)(ArgTypes...), method, Args<>, Args<ArgTypes...> >::Call(&state, thisObject));
		}
		catch(Exception* exception)
		{
			// положить в стек исключение
			State::Push(state, MakePointer(exception));
			// и выбросить ошибку
			lua_error(state);
		}
	}
};

/// Структура превращения конструкторов в функции lua.
template <typename ClassType, typename... ArgTypes>
struct ConstructorThunk
{
	/// Функция, создающая объект вызовом конструктора.
	/** Следует формату и протоколу lua_CFunction. */
	static int Thunk(lua_State* luaState)
	{
		try
		{
			// сформировать состояние для получения аргументов
			ArgGettingState state;
			state.state = luaState;
			// получить действительное количество аргументов
			state.argsCount = lua_gettop(luaState);
			state.gotArgsCount = 0;

			// получить аргументы, выполнить вызов и положить результат в стек
			// возвращается количество результатов
			return ArgPusher<ptr<ClassType> >::Push(luaState, ConstructorCaller<ClassType, Args<>, Args<ArgTypes...> >::Call(&state));
		}
		catch(Exception* exception)
		{
			// положить в стек исключение
			State::Push(state, MakePointer(exception));
			// и выбросить ошибку
			lua_error(state);
		}
	}
};

END_INANITY_LUA

#endif
