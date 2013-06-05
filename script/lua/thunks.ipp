#ifndef ___INANITY_SCRIPT_LUA_THUNKS_IPP___
#define ___INANITY_SCRIPT_LUA_THUNKS_IPP___

#include "thunks.hpp"
#include "callable.hpp"
#include "values.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_LUA

/// Вспомогательная структура, вызывающая статический Call у переданного типа,
/// и кладущая результат в стек. Корректно обрабатывает возвращаемое значение void.
template <typename CallerType, typename ReturnType, typename Args>
struct CallAndReturn
{
	static inline int Do(ArgGettingState& state, Args& args)
	{
		Value<ReturnType>::Push(state.state, CallerType::Call(state, args, args));
		return 1;
	}
};
template <typename CallerType, typename Args>
struct CallAndReturn<CallerType, void, Args>
{
	static inline int Do(ArgGettingState& state, Args& args)
	{
		CallerType::Call(state, args, args);
		return 0;
	}
};

/// Получить значение из ArgGettingState, и увеличить счётчик.
template <typename ArgType>
struct ArgGetter
{
	static inline auto Get(ArgGettingState& state) -> decltype(Value<ArgType>::Get(state.state, ++state.gotArgsCount))
	{
		return Value<ArgType>::Get(state.state, ++state.gotArgsCount);
	}
};

/// Преобразователь, возвращающий для void TypesVoid.
template <typename ArgsType>
struct ArgsOrVoid
{
	typedef ArgsType Args;
};
template <>
struct ArgsOrVoid<void>
{
	typedef TypesVoid Args;
};

/// Вспомогательная структура, выполняющая накопление аргументов и вызов.
template <typename CalleeType, CalleeType callee, typename RestArgs>
struct Caller;
// когда все аргументы получены
template <typename CalleeType, CalleeType callee>
struct Caller<CalleeType, callee, void>
{
	static inline typename Callable<CalleeType>::ReturnType Call(ArgGettingState& state, typename ArgsOrVoid<typename Callable<CalleeType>::Args>::Args& args, TypesVoid& restArgs)
	{
		// в стеке не должно остаться аргументов
		if(state.gotArgsCount != state.argsCount)
			THROW_PRIMARY_EXCEPTION("Extra arguments for function call");

		// выполнить вызов
		return Callable<CalleeType>::template Call<callee>(args);
	}
};
// когда ещё не все аргументы получены
template <typename CalleeType, CalleeType callee, typename FirstRestArg, typename RestRestArgs>
struct Caller<CalleeType, callee, Types<FirstRestArg, RestRestArgs> >
{
	static inline typename Callable<CalleeType>::ReturnType Call(ArgGettingState& state, typename Callable<CalleeType>::Args& args, Types<FirstRestArg, RestRestArgs>& restArgs)
	{
		restArgs.first = ArgGetter<FirstRestArg>::Get(state);
		return Caller<CalleeType, callee, RestRestArgs>::Call(state, args, restArgs.rest);
	}
};

/// Вспомогательная структура, выполняющая накопление аргументов и вызов конструктора.
template <typename CalleeType, typename RestArgs>
struct ConstructorCaller;
// когда все аргументы получены
template <typename CalleeType>
struct ConstructorCaller<CalleeType, void>
{
	static inline typename CallableConstructor<CalleeType>::ReturnType Call(ArgGettingState& state, typename ArgsOrVoid<typename CallableConstructor<CalleeType>::Args>::Args& args, TypesVoid& restArgs)
	{
		// в стеке не должно остаться аргументов
		if(state.gotArgsCount != state.argsCount)
			THROW_PRIMARY_EXCEPTION("Extra arguments for function call");

		// выполнить вызов
		return CallableConstructor<CalleeType>::Call(args);
	}
};
// когда ещё не все аргументы получены
template <typename CalleeType, typename FirstRestArg, typename RestRestArgs>
struct ConstructorCaller<CalleeType, Types<FirstRestArg, RestRestArgs> >
{
	static inline typename CallableConstructor<CalleeType>::ReturnType Call(ArgGettingState& state, typename CallableConstructor<CalleeType>::Args& args, Types<FirstRestArg, RestRestArgs>& restArgs)
	{
		restArgs.first = ArgGetter<FirstRestArg>::Get(state);
		return ConstructorCaller<CalleeType, RestRestArgs>::Call(state, args, restArgs.rest);
	}
};

template <typename CalleeType, CalleeType callee>
struct CalleeThunk
{
	static inline int Thunk(lua_State* luaState)
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
			typename ArgsOrVoid<typename Callable<CalleeType>::Args>::Args args;
			return CallAndReturn<Caller<CalleeType, callee, typename Callable<CalleeType>::Args>, typename Callable<CalleeType>::ReturnType, typename ArgsOrVoid<typename Callable<CalleeType>::Args>::Args>::Do(state, args);
		}
		catch(Exception* exception)
		{
			// положить в стек исключение
			Value<ptr<Exception> >::Push(luaState, exception);
			// и выбросить ошибку
			lua_error(luaState);
			// lua_error не возвращает управления, это только для компилятора
			return 0;
		}
	}
};

template <typename CalleeType>
struct ConstructorThunk
{
	static inline int Thunk(lua_State* luaState)
	{
		try
		{
			// сформировать состояние для получения аргументов
			ArgGettingState state;
			state.state = luaState;
			// получить действительное количество аргументов
			state.argsCount = lua_gettop(luaState);
			// первый аргумент нужно пропустить, так как это всегда будет объект класса
			state.gotArgsCount = 1;

			// получить аргументы, выполнить вызов и положить результат в стек
			// возвращается количество результатов
			typename ArgsOrVoid<typename CallableConstructor<CalleeType>::Args>::Args args;
			return CallAndReturn<ConstructorCaller<CalleeType, typename CallableConstructor<CalleeType>::Args>, typename CallableConstructor<CalleeType>::ReturnType, typename ArgsOrVoid<typename CallableConstructor<CalleeType>::Args>::Args>::Do(state, args);
		}
		catch(Exception* exception)
		{
			// положить в стек исключение
			Value<ptr<Exception> >::Push(luaState, exception);
			// и выбросить ошибку
			lua_error(luaState);
			// lua_error не возвращает управления, это только для компилятора
			return 0;
		}
	}
};

END_INANITY_LUA

#endif
