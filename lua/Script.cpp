#include "Script.ipp"

BEGIN_INANITY_LUA

Script::Script(ptr<State> state) : state(state)
{
	lua_State* luaState = state->GetState();

	// забрать функцию из стека и положить её в реестр по нашему адресу
	lua_pushlightuserdata(luaState, this);             // function this
	lua_insert(luaState, lua_absindex(luaState, -2));   // this function
	lua_settable(luaState, LUA_REGISTRYINDEX);
}

Script::~Script()
{
	lua_State* luaState = state->GetState();

	// удалить функцию из реестра
	lua_pushlightuserdata(luaState, this);
	lua_pushnil(luaState);
	lua_settable(luaState, LUA_REGISTRYINDEX);
}

END_INANITY_LUA
