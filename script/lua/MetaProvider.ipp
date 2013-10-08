#ifndef ___INANITY_SCRIPT_LUA_META_PROVIDER_IPP___
#define ___INANITY_SCRIPT_LUA_META_PROVIDER_IPP___

#include "MetaProvider.hpp"
#include "../../meta/ClassBase.ipp"
#include "thunks.ipp"

BEGIN_INANITY_LUA

template <typename ConstructorType>
void MetaProvider::Constructor<ConstructorType>::PushThunk(lua_State* luaState)
{
	lua_pushcclosure(luaState, &ConstructorThunk<ConstructorType>::Thunk, 0);
}

template <typename FunctionType, FunctionType function>
void MetaProvider::Function<FunctionType, function>::PushThunk(lua_State* luaState)
{
	lua_pushcclosure(luaState, &CalleeThunk<FunctionType, function>::Thunk, 0);
}

template <typename MethodType, MethodType method>
void MetaProvider::Method<MethodType, method>::PushThunk(lua_State* luaState)
{
	lua_pushcclosure(luaState, &CalleeThunk<MethodType, method>::Thunk, 0);
}

END_INANITY_LUA

#endif
