#include "Function.hpp"
#include "State.hpp"
#include "stuff.hpp"
#include "../../Exception.hpp"

BEGIN_INANITY_LUA

Function::Function(ptr<State> state) : state(state)
{
	lua_State* luaState = state->GetState();

	// pop the function from the stack and store it to registry
	lua_pushlightuserdata(luaState, this);              // function this
	lua_insert(luaState, lua_absindex(luaState, -2));   // this function
	lua_settable(luaState, LUA_REGISTRYINDEX);
}

Function::~Function()
{
	lua_State* luaState = state->GetState();

	// remove function from registry
	lua_pushlightuserdata(luaState, this);
	lua_pushnil(luaState);
	lua_settable(luaState, LUA_REGISTRYINDEX);
}

void Function::Run()
{
	lua_State* luaState = state->GetState();

#ifdef _DEBUG
	/* A special object, which checks that stack is balanced
	after returning from function. Because Run routine works
	in a global stack, that is especially matter. */
	struct StackBalanceCheck
	{
		lua_State* state;
		int top;
		StackBalanceCheck(lua_State* state) : state(state), top(lua_gettop(state)) {}
		~StackBalanceCheck()
		{
			if(top != lua_gettop(state))
				THROW("Script::Run - unbalanced Lua stack");
		}
	};
	StackBalanceCheck check(luaState);
#endif

	// push error handler function
	lua_pushcclosure(luaState, ScriptErrorHook, 0);
	// save index of error handler
	int errorHookIndex = lua_gettop(luaState);
	// save current stack
	int beginStack = lua_gettop(luaState);
	// push function in stack
	lua_pushlightuserdata(luaState, this);
	lua_gettable(luaState, LUA_REGISTRYINDEX);

	// perform call
	int code = lua_pcall(luaState, 0 /*sizeof...(args)*/, LUA_MULTRET, errorHookIndex);

	// get amount of results
	int argsCount = lua_gettop(luaState) - beginStack;

	// remove error handler from stack
	lua_remove(luaState, errorHookIndex);

	// if the call succeeded
	if(code == LUA_OK)
	{
		if(argsCount != 0)
			THROW("Lua function expected to return no results");
		return;
	}

	// else there is an error on stack, process it
	ProcessError(luaState);
}

END_INANITY_LUA
