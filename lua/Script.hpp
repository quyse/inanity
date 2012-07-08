#ifndef ___INANITY_LUA_SCRIPT_HPP___
#define ___INANITY_LUA_SCRIPT_HPP___

#include "lua.hpp"
#include "State.hpp"
#include "core.hpp"

BEGIN_INANITY_LUA

class State;

/// Класс функции Lua.
/** Хранит собственно функцию в Lua Registry, по адресу объекта Function. */
class Script : public Object
{
private:
	ptr<State> state;

public:
	/// Создать скрипт Lua.
	/** Предполагается, что при вызове в стеке Lua лежит функция скрипта. Конструктор забирает её из стека. */
	Script(ptr<State> state);
	~Script();

	/// Вызвать функцию с заданными параметрами.
	template <typename ReturnType, typename... ArgTypes>
	ReturnType Run(ArgTypes... args)
	{
		lua_State* luaState = state->GetState();
		// запомнить текущий стек
		int beginStack = lua_gettop(luaState);
		// запихать в стек функцию
		lua_pushlightuserdata(luaState, this);
		lua_gettable(luaState, LUA_REGISTRYINDEX);
		// запихать в стек аргументы
		ArgsPusher<ArgTypes...>::Push(luaState, args...);
		// выполнить вызов
		if(lua_pcall(luaState, sizeof...(args), LUA_MULTRET, 0) == LUA_OK)
		{
			// получить, сколько результатов вернулось
			ArgGettingState gettingState;
			gettingState.state = luaState;
			gettingState.argsCount = lua_gettop(luaState) - beginStack;
			gettingState.gotArgsCount = 0;
#if 0 // FIXME: проверять всё-таки надо, но загвоздка с void
			// получить единственный результат
			ReturnType result = NextArgGetter<ReturnType>::Get(&gettingState);
			// проверить, что результатов правильное количество
			if(gettingState.gotArgsCount != gettingState.argsCount)
				THROW_PRIMARY_EXCEPTION("Invalid results count returned by Lua function");
			// вернуть результат
			return result;
#else
			// просто получить
			return NextArgGetter<ReturnType>::Get(&gettingState);
#endif
		}
		// иначе ошибка, обработать её
		ProcessError(luaState);
	}
};

END_INANITY_LUA

#endif
