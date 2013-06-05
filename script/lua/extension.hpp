#ifndef ___INANITY_SCRIPT_LUA_EXTENSION_HPP___
#define ___INANITY_SCRIPT_LUA_EXTENSION_HPP___

#include "lua.hpp"
#include "../../meta/Constructor.hpp"
#include "../../meta/Function.hpp"
#include "../../meta/Method.hpp"
#include "thunks.hpp"

BEGIN_INANITY_LUA

template <typename Type>
class Extension;

template <typename ClassType>
class Extension<Meta::Class<ClassType> >
{
};

template <typename ConstructorType>
class Extension<Meta::Constructor<ConstructorType> >
{
public:
	void PushThunk(lua_State* luaState)
	{
		lua_pushcclosure(luaState, &ConstructorThunk<ConstructorType>::Thunk, 0);
	}
};

template <typename FunctionType, FunctionType function>
class Extension<Meta::Function<FunctionType, function> >
{
public:
	void PushThunk(lua_State* luaState)
	{
		lua_pushcclosure(luaState, &CalleeThunk<FunctionType, function>::Thunk, 0);
	}
};

template <typename MethodType, MethodType method>
class Extension<Meta::Method<MethodType, method> >
{
public:
	void PushThunk(lua_State* luaState)
	{
		lua_pushcclosure(luaState, &CalleeThunk<MethodType, method>::Thunk, 0);
	}
};

END_INANITY_LUA

#endif
