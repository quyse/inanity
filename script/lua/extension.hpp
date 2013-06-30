#ifndef ___INANITY_SCRIPT_LUA_EXTENSION_HPP___
#define ___INANITY_SCRIPT_LUA_EXTENSION_HPP___

#include "lua.hpp"
#include "thunks.hpp"
#include "../../meta/extension_decl.hpp"

BEGIN_INANITY_LUA

template <typename Type>
class Extension;

class ClassExtensionBase
{
};

/// Class meta extension for Lua.
template <typename ClassType>
class Extension<Meta::Class<ClassType> > : public ClassExtensionBase
{
};

class CallableExtensionBase
{
public:
	virtual void PushThunk(lua_State* luaState) = 0;
};

typedef CallableExtensionBase ConstructorExtensionBase;

template <typename ConstructorType>
class Extension<Meta::Constructor<ConstructorType> > : public ConstructorExtensionBase
{
public:
	void PushThunk(lua_State* luaState);
};

typedef CallableExtensionBase FunctionExtensionBase;

template <typename FunctionType, FunctionType function>
class Extension<Meta::Function<FunctionType, function> > : public FunctionExtensionBase
{
public:
	void PushThunk(lua_State* luaState);
};

typedef CallableExtensionBase MethodExtensionBase;

template <typename MethodType, MethodType method>
class Extension<Meta::Method<MethodType, method> > : public MethodExtensionBase
{
public:
	void PushThunk(lua_State* luaState);
};

END_INANITY_LUA

#endif
