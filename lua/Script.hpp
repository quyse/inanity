#ifndef ___INANITY_LUA_SCRIPT_HPP___
#define ___INANITY_LUA_SCRIPT_HPP___

#include "Script_decl.hpp"
#include "values_decl.hpp"
#include "State.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_LUA

template <typename ReturnType>
inline ReturnType ScriptReturn(lua_State* state, int argsCount)
{
	if(argsCount != 1)
		THROW_PRIMARY_EXCEPTION("Lua function expected to return exactly one result");
	return Value<ReturnType>::Get(state, -1);
}
template <>
inline void ScriptReturn<void>(lua_State* state, int argsCount)
{
	if(argsCount != 0)
		THROW_PRIMARY_EXCEPTION("Lua function expected to return no results");
}

template <typename ReturnType>
ReturnType Script::Run()
{
	lua_State* luaState = state->GetState();
	// запомнить текущий стек
	int beginStack = lua_gettop(luaState);
	// запихать в стек функцию
	lua_pushlightuserdata(luaState, this);
	lua_gettable(luaState, LUA_REGISTRYINDEX);
	// запихать в стек аргументы
	//ArgsPusher<ArgTypes...>::Push(luaState, args...);
	// выполнить вызов
	if(lua_pcall(luaState, 0 /*sizeof...(args)*/, LUA_MULTRET, 0) == LUA_OK)
	{
		// получить, сколько результатов вернулось
		int argsCount = lua_gettop(luaState) - beginStack;
		// вернуть результат, если он есть, и проверить правильность количества аргументов
		return ScriptReturn<ReturnType>(luaState, argsCount);
	}
	// иначе ошибка, обработать её
	ProcessError(luaState);
}

END_INANITY_LUA

#endif
