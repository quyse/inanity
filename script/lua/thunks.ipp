#ifndef ___INANITY_SCRIPT_LUA_THUNKS_IPP___
#define ___INANITY_SCRIPT_LUA_THUNKS_IPP___

#include "thunks.hpp"
#include "values.hpp"
#include "stuff.hpp"
#include "lualib.hpp"
#include "../../meta/Tuple.hpp"
#include "../../meta/Callable.hpp"
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
			// выбросить ошибку Lua
			ThrowError(luaState, exception);
			// управление не возвращается, это только для компилятора
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
			// выбросить ошибку Lua
			ThrowError(luaState, exception);
			// управление не возвращается, это только для компилятора
			return 0;
		}
	}
};

END_INANITY_LUA

#endif
