#ifndef ___INANITY_SCRIPT_LUA_META_PROVIDER_IPP___
#define ___INANITY_SCRIPT_LUA_META_PROVIDER_IPP___

#include "MetaProvider.hpp"
#include "../../meta/ClassBase.hpp"
#include "thunks.hpp"

BEGIN_INANITY_LUA

//*** class MetaProvider::Constructor

template <typename ClassType, typename... Args>
void MetaProvider::Constructor<ClassType, Args...>::PushThunk(lua_State* luaState)
{
	lua_pushcclosure(luaState, &ConstructorThunk<ClassType, Args...>::Thunk, 0);
}

//*** class MetaProvider::Function

template <typename FunctionType, FunctionType function>
MetaProvider::Function<FunctionType, function>::Function(const char* name)
: FunctionBase(name) {}

template <typename FunctionType, FunctionType function>
void MetaProvider::Function<FunctionType, function>::PushThunk(lua_State* luaState)
{
	lua_pushcclosure(luaState, &CalleeThunk<FunctionType, function>::Thunk, 0);
}

//*** class MetaProvider::Method

template <typename MethodType, MethodType method>
MetaProvider::Method<MethodType, method>::Method(const char* name)
: MethodBase(name) {}

template <typename MethodType, MethodType method>
void MetaProvider::Method<MethodType, method>::PushThunk(lua_State* luaState)
{
	lua_pushcclosure(luaState, &CalleeThunk<MethodType, method>::Thunk, 0);
}

//*** class MetaProvider::Class

template <typename ClassType>
MetaProvider::Class<ClassType>::Class(const char* name, const char* fullName)
: ClassBase(name, fullName) {}

END_INANITY_LUA

#endif
