#ifndef ___INANITY_LUA_STUFF_HPP___
#define ___INANITY_LUA_STUFF_HPP___

/* Файл содержит различные шаблонные функции.
Часто функции оформляются как статические методы в структурах,
только потому, что функции не могут иметь частичных специализаций.
*/

#include "lua.hpp"
#include "lualib.hpp"
#include "State.hpp"
#include "userdata.hpp"
#include "reflection_decl.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_LUA

/// Универсальная функция для индексирования метатаблиц (классов и объектов).
/** Предполагает, что в замыкании лежит таблица методов. */
int MetaTable_index(lua_State* state);

/// Запихать в стек метатаблицу для классов.
/** Метатаблица классов нигде не хранится, и создаётся каждый раз заново.
Потому что предполагается, что она один раз будет нужна (для одного глобального объекта).
Вообще говоря, можно было бы обойтись вообще без метатаблицы, а сделать просто таблицу,
а не объект. Но пофиг, пусть будет более круто. */
void PushClassMetaTable(lua_State* state, Class& cls);

/// Зарегистрировать класс в состоянии.
void RegisterClass(lua_State* state, Class& cls);

/// Структура с методами метатаблицы объектов.
int ObjectMetaTable_gc(lua_State* state);

/// Запихать в стек метатаблицу для объектов.
void PushObjectMetaTable(lua_State* state, Class& cls);

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
template <>
struct ArgGetter<const String&>
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
		ObjectUserData* userData = (ObjectUserData*)lua_touserdata(state, index);
		if(!userData || lua_islightuserdata(state, index) || userData->type != UserData::typeObject)
			THROW_PRIMARY_EXCEPTION("Expected an object for argument");
		// проверить тип объекта
		if(&ObjectType::scriptClass != userData->cls)
			THROW_PRIMARY_EXCEPTION("An object of another type was expected for argument");
		// вернуть объект
		return (ObjectType*)userData->object;
	}
};

/// Вспомогательная функция для получения аргумента из стека Lua.
/** Использует структуру для получения кучи аргументов, и увеличивает счётчик. */
template <typename ArgType>
struct NextArgGetter
{
	static inline auto Get(ArgGettingState* state) -> decltype(ArgGetter<ArgType>::Get(state->state, ++state->gotArgsCount))
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

/// Вспомогательная структура для запихивания аргумента в стек Lua.
template <typename ArgType>
struct ArgPusher;
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
	static int Push(lua_State* state, const char* arg)
	{
		lua_pushstring(state, arg);
		return 1;
	}
};
template <>
struct ArgPusher<String>
{
	static int Push(lua_State* state, const String& arg)
	{
		lua_pushstring(state, arg.c_str());
		return 1;
	}
};
template <typename ObjectType>
struct ArgPusher<ptr<ObjectType> >
{
	static int Push(lua_State* state, ptr<ObjectType> arg)
	{
		ObjectUserData* userData = (ObjectUserData*)lua_newuserdata(state, sizeof(ObjectUserData));
		userData->type = UserData::typeObject;
		userData->object = (Object*)(ObjectType*)arg;
		userData->cls = &ObjectType::scriptClass;
		// указать метатаблицу
		PushObjectMetaTable(state, ObjectType::scriptClass);
		lua_setmetatable(state, -2);
		// задать дополнительную ссылку объекту
		userData->object->Reference();
		return 1;
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

/// Вспомогательная структура, вызывающая статический Call у переданного типа,
/// и кладущая результат в стек. Корректно обрабатывает void.
template <typename CallerType, typename ReturnType, typename... ArgTypes>
struct CallAndPusher
{
	static inline int CallAndPush(lua_State* state, ArgTypes... args)
	{
		return ArgPusher<ReturnType>::Push(state, CallerType::Call(args...));
	}
};
template <typename CallerType, typename... ArgTypes>
struct CallAndPusher<CallerType, void, ArgTypes...>
{
	static inline int CallAndPush(lua_State* state, ArgTypes... args)
	{
		CallerType::Call(args...);
		return 0;
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
		return FunctionCaller<ReturnType (*)(GotArgTypes..., FirstRestArgType, MoreRestArgTypes...), function, Args<GotArgTypes..., FirstRestArgType>, Args<MoreRestArgTypes...> >::Call(gotArgs..., NextArgGetter<FirstRestArgType>::Get(state));
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
		return MethodCaller<ReturnType (ClassType::*)(GotArgTypes..., FirstRestArgType, MoreRestArgTypes...), method, Args<GotArgTypes..., FirstRestArgType>, Args<MoreRestArgTypes...> >::Call(state, object, gotArgs..., NextArgGetter<FirstRestArgType>::Get(state));
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
			//return ArgPusher<ReturnType>::Push(luaState, FunctionCaller<ReturnType (*)(ArgTypes...), function, Args<>, Args<ArgTypes...> >::Call(&state));
			return CallAndPusher<FunctionCaller<ReturnType (*)(ArgTypes...), function, Args<>, Args<ArgTypes...> >, ReturnType, ArgGettingState*>(luaState, &state);
		}
		catch(Exception* exception)
		{
			// положить в стек исключение
			ArgPusher<ptr<Exception> >::Push(luaState, exception);
			// и выбросить ошибку
			lua_error(luaState);
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
			return CallAndPusher<MethodCaller<ReturnType (ClassType::*)(ArgTypes...), method, Args<>, Args<ArgTypes...> >, ReturnType, ArgGettingState*, ptr<ClassType> >::CallAndPush(luaState, &state, thisObject);
		}
		catch(Exception* exception)
		{
			// положить в стек исключение
			ArgPusher<ptr<Exception> >::Push(luaState, exception);
			// и выбросить ошибку
			lua_error(luaState);
			// lua_error не возвращает управления, это только для компилятора
			return 0;
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
			ArgPusher<ptr<Exception> >::Push(luaState, exception);
			// и выбросить ошибку
			lua_error(luaState);
		}
	}
};

END_INANITY_LUA

#endif
