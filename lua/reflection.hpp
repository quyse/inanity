#ifndef ___INANITY_LUA_REFLECTION_HPP___
#define ___INANITY_LUA_REFLECTION_HPP___

#include "reflection_decl.hpp"
#include "core.hpp"
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

// нестатический метод
template <typename ReturnType, typename ClassType, typename... ArgTypes, ReturnType (ClassType::*method)(ArgTypes...)>
struct ClassMethodAdder<ReturnType (ClassType::*)(ArgTypes...), method>
{
	static void Add(Class* cls, const char* name)
	{
		cls->methods.push_back(new ConcreteMethod<ReturnType (ClassType::*)(ArgTypes...), method>(name));
	}
};
// статический метод
template <typename ReturnType, typename... ArgTypes, ReturnType (*method)(ArgTypes...)>
struct ClassMethodAdder<ReturnType (*)(ArgTypes...), method>
{
	static void Add(Class* cls, const char* name)
	{
		cls->staticMethods.push_back(new ConcreteFunction<ReturnType (*)(ArgTypes...), method>(name));
	}
};

template <typename MethodType, MethodType method>
void Class::AddMethod(const char* name)
{
	ClassMethodAdder<MethodType, method>::Add(this, name);
}

END_INANITY_LUA

#endif
