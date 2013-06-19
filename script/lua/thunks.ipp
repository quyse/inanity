#ifndef ___INANITY_SCRIPT_LUA_THUNKS_IPP___
#define ___INANITY_SCRIPT_LUA_THUNKS_IPP___

#include "thunks.hpp"
#include "values.ipp"
#include "../../meta/Tuple.hpp"
#include "../../meta/Callable.ipp"
#include "../../Exception.hpp"

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

	/// Метод для инициализации Tuple.
	template <typename ArgType>
	inline typename Value<ArgType>::ValueType Get()
	{
		return Value<ArgType>::Get(state, ++gotArgsCount);
	}
};

/// Вспомогательная структура, вызывающая статический Call у переданного типа,
/// и кладущая результат в стек. Корректно обрабатывает возвращаемое значение void.
template <typename CallerType, typename ReturnType, typename Args>
struct CallAndReturn
{
	static inline int Do(lua_State* state, const Args& args)
	{
		Value<ReturnType>::Push(state, CallerType::Call(args));
		return 1;
	}
};
template <typename CallerType, typename Args>
struct CallAndReturn<CallerType, void, Args>
{
	static inline int Do(lua_State* state, const Args& args)
	{
		CallerType::Call(args);
		return 0;
	}
};

/*
/// Вспомогательная структура, выполняющая накопление аргументов и вызов.
template <typename CalleeType, CalleeType callee, typename RestArgs>
struct Caller;
// когда все аргументы получены
template <typename CalleeType, CalleeType callee>
struct Caller<CalleeType, callee, void>
{
	static inline typename Meta::Callable<CalleeType>::ReturnType Call(ArgGettingState& state, typename Meta::Callable<CalleeType>::Args& args, Meta::VoidTuple& restArgs)
	{
		// в стеке не должно остаться аргументов
		if(state.gotArgsCount != state.argsCount)
			THROW_PRIMARY_EXCEPTION("Extra arguments for function call");

		// выполнить вызов
		return Meta::Callable<CalleeType>::template Call<callee>(args);
	}
};
// когда ещё не все аргументы получены
template <typename CalleeType, CalleeType callee, typename FirstRestArg, typename RestRestArgs>
struct Caller<CalleeType, callee, Meta::Tuple<FirstRestArg, RestRestArgs> >
{
	static inline typename Meta::Callable<CalleeType>::ReturnType Call(ArgGettingState& state, typename Meta::Callable<CalleeType>::Args& args, Meta::Tuple<FirstRestArg, RestRestArgs>& restArgs)
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
	static inline typename Meta::CallableConstructor<CalleeType>::ReturnType Call(ArgGettingState& state, typename Meta::CallableConstructor<CalleeType>::Args& args, Meta::VoidTuple& restArgs)
	{
		// в стеке не должно остаться аргументов
		if(state.gotArgsCount != state.argsCount)
			THROW_PRIMARY_EXCEPTION("Extra arguments for function call");

		// выполнить вызов
		return Meta::CallableConstructor<CalleeType>::Call(args);
	}
};
// когда ещё не все аргументы получены
template <typename CalleeType, typename FirstRestArg, typename RestRestArgs>
struct ConstructorCaller<CalleeType, Meta::Tuple<FirstRestArg, RestRestArgs> >
{
	static inline typename Meta::CallableConstructor<CalleeType>::ReturnType Call(ArgGettingState& state, typename Meta::CallableConstructor<CalleeType>::Args& args, Meta::Tuple<FirstRestArg, RestRestArgs>& restArgs)
	{
		restArgs.first = ArgGetter<FirstRestArg>::Get(state);
		return ConstructorCaller<CalleeType, RestRestArgs>::Call(state, args, restArgs.rest);
	}
};
*/

template <typename CalleeType, CalleeType callee>
struct CalleeThunk
{
	typedef typename Meta::Callable<CalleeType>::Args Args;
	typedef typename Meta::Callable<CalleeType>::ReturnType ReturnType;

	struct Helper
	{
		static inline ReturnType Call(const Args& args)
		{
			return Meta::Callable<CalleeType>::Call(callee, args);
		}
	};

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
			return CallAndReturn<Helper, ReturnType, Args>::Do(luaState, Args(state));
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
	typedef typename Meta::CallableConstructor<CalleeType>::Args Args;
	typedef typename Meta::CallableConstructor<CalleeType>::ReturnType ReturnType;

	struct Helper
	{
		static inline ReturnType Call(const Args& args)
		{
			return Meta::CallableConstructor<CalleeType>::Call(args);
		}
	};

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
			return CallAndReturn<Helper, ReturnType, Args>::Do(luaState, Args(state));
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
