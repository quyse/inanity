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

#ifdef _DEBUG
	/* специальный объект, проверяющий, что стек остался
	сбалансированным после выхода из функции
	именно для Script::Run это особенно важно, так как
	она работает в "глобальном" стеке, и никто за неё
	его не сбалансирует. */
	struct StackBalanceCheck
	{
		lua_State* state;
		int top;
		StackBalanceCheck(lua_State* state) : state(state), top(lua_gettop(state)) {}
		~StackBalanceCheck()
		{
			if(top != lua_gettop(state))
				THROW_PRIMARY_EXCEPTION("Script::Run - unbalanced Lua stack");
		}
	};
	StackBalanceCheck check(luaState);
#endif

	// запихать в стек функцию обработки ошибок
	lua_pushcclosure(luaState, ScriptErrorHook, 0);
	// запомнить индекс обработчика
	int errorHookIndex = lua_gettop(luaState);
	// запомнить текущий стек
	int beginStack = lua_gettop(luaState);
	// запихать в стек функцию
	lua_pushlightuserdata(luaState, this);
	lua_gettable(luaState, LUA_REGISTRYINDEX);

	// выполнить вызов
	int code = lua_pcall(luaState, 0 /*sizeof...(args)*/, LUA_MULTRET, errorHookIndex);

	// получить, сколько результатов вернулось
	int argsCount = lua_gettop(luaState) - beginStack;

	// удалить ранее запихнутую функцию
	lua_remove(luaState, errorHookIndex);

	// если вызов был удачен
	if(code == LUA_OK)
		// вернуть результат, если он есть, и проверить правильность количества аргументов
		return ScriptReturn<ReturnType>(luaState, argsCount);

	// иначе на стеке ошибка, обработать её
	ProcessError(luaState);
}

END_INANITY_LUA

#endif
