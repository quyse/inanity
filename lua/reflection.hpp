#ifndef ___INANITY_LUA_REFLECTION_HPP___
#define ___INANITY_LUA_REFLECTION_HPP___

#include "reflection_decl.hpp"
#include "stuff.hpp"
#include "lualib.hpp"

BEGIN_INANITY_LUA

template <typename FunctionType, FunctionType function>
void ConcreteFunction<FunctionType, function>::PushThunk(lua_State* luaState)
{
	lua_pushcclosure(luaState, &FunctionThunk<FunctionType, function>::Thunk, 0);
}

template <typename MethodType, MethodType method>
void ConcreteMethod<MethodType, method>::PushThunk(lua_State* luaState)
{
	lua_pushcclosure(luaState, &MethodThunk<MethodType, method>::Thunk, 0);
}

template <typename ClassType, typename... ArgTypes>
void ConcreteConstructor<ClassType, ArgTypes...>::PushThunk(lua_State* luaState)
{
	lua_pushcclosure(luaState, &ConstructorThunk<ClassType, ArgTypes...>::Thunk, 0);
}

template <typename ClassType, typename... ArgTypes>
void Class::SetConstructor()
{
	if(constructor)
		delete constructor;
	constructor = new ConcreteConstructor<ClassType, ArgTypes...>();
}

template <typename MethodType, MethodType method>
void Class::AddMethod(const char* name)
{
	methods.push_back(new ConcreteMethod<MethodType, method>(name));
}

template <typename FunctionType, FunctionType function>
void Class::AddStaticMethod(const char* name)
{
	staticMethods.push_back(new ConcreteFunction<FunctionType, function>(name));
}

END_INANITY_LUA

#endif
