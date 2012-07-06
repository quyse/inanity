#ifndef ___INANITY_LUA_CALLING_HPP___
#define ___INANITY_LUA_CALLING_HPP___

#include "lua.hpp"
#include "lualib.hpp"
#include "State.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_LUA

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

/// Вспомогательная структура для получения аргумента из стека Lua.
template <typename ArgType>
ArgType GetNextArg(ArgGettingState* state);

template <>
bool GetNextArg<bool>(ArgGettingState* state)
{
	return lua_toboolean(state->state, ++state->gotArgsCount);
}

template <>
int GetNextArg<int>(ArgGettingState* state)
{
	int isnum;
	int res = lua_tointegerx(state->state, ++state->gotArgsCount, &isnum);
	if(!isnum)
		THROW_PRIMARY_EXCEPTION("Expected an integer for argument");
	return res;
}

template <>
const char* GetNextArg<const char*>(ArgGettingState* state)
{
	const char* res = lua_tostring(state->state, ++state->gotArgsCount);
	if(!res)
		THROW_PRIMARY_EXCEPTION("Expected a string for argument");
	return res;
}

template <>
String GetNextArg<String>(ArgGettingState* state)
{
	return GetNextArg<const char*>(state);
}

/// Структура-контейнер списка типов.
/** Только объявление шаблона, определение не требуется. */
template <typename... ArgTypes>
struct Args;

/// Вспомогательная структура для вызова метода.
template <typename MethodType, MethodType method, typename GotArgs, typename RestArgs>
struct MethodCaller;
// Когда все аргументы уже получены.
template <typename ReturnType, typename ClassType, typename... ArgTypes, ReturnType (ClassType::*method)(ArgTypes...)>
struct MethodCaller<ReturnType (ClassType::*)(ArgTypes...), method, Args<ArgTypes...>, Args<> >
{
	inline static ReturnType call(ArgGettingState* state, ClassType* object, ArgTypes... args)
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
	inline static ReturnType Call(ArgGettingState* state, GotArgTypes... gotArgs)
	{
		// в стеке должен быть аргумент
		if(state->gotArgsCount >= state->argsCount)
			THROW_PRIMARY_EXCEPTION("Not enough arguments for method call");
		// аргумент есть, получить его и двигаться дальше
		return MethodCaller<ReturnType (ClassType::*)(GotArgTypes..., FirstRestArgType, MoreRestArgTypes...), method, Args<GotArgTypes..., FirstRestArgType>, Args<MoreRestArgTypes...> >::Call(gotArgs..., GetNextArg<FirstRestArgType>(state));
	}
};


/// Структура превращения методов в функции-методы lua.
/** Структура, потому что функции не могут иметь частичные специализации. */
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
			ptr<ClassType> thisObject = GetNextArg<ptr<ClassType> >(&state);

			// получить аргументы, выполнить вызов и положить результат в стек
			Push(MethodCaller<ReturnType (ClassType::*)(ArgTypes...), method, Args<>, Args<ArgTypes...> >::Call(&state));

			// вернуть количество результатов (void пока не предусмотрен)
			return 1;
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
