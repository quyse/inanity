#ifndef ___INANITY_SCRIPT_LUA_EXTENSION_IPP___
#define ___INANITY_SCRIPT_LUA_EXTENSION_IPP___

#include "extension.hpp"
#include "thunks.ipp"
#include "../../meta/Class.ipp"
#include "../../meta/Constructor.ipp"
#include "../../meta/Method.ipp"
#include "../../meta/Function.ipp"

BEGIN_INANITY_LUA

template <typename ConstructorType>
void Extension<Meta::Constructor<ConstructorType> >::PushThunk(lua_State* luaState)
{
	lua_pushcclosure(luaState, &ConstructorThunk<ConstructorType>::Thunk, 0);
}

template <typename FunctionType, FunctionType function>
void Extension<Meta::Function<FunctionType, function> >::PushThunk(lua_State* luaState)
{
	lua_pushcclosure(luaState, &CalleeThunk<FunctionType, function>::Thunk, 0);
}

template <typename MethodType, MethodType method>
void Extension<Meta::Method<MethodType, method> >::PushThunk(lua_State* luaState)
{
	lua_pushcclosure(luaState, &CalleeThunk<MethodType, method>::Thunk, 0);
}

END_INANITY_LUA

#endif
