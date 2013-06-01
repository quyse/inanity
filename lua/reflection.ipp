#ifndef ___INANITY_LUA_REFLECTION_IPP___
#define ___INANITY_LUA_REFLECTION_IPP___

#include "reflection.hpp"
#include "thunks.hpp"
#include "callable.hpp"
#include "lualib.hpp"

BEGIN_INANITY_LUA

template <typename FunctionType, FunctionType function>
void ConcreteFunction<FunctionType, function>::PushThunk(lua_State* luaState)
{
	lua_pushcclosure(luaState, &CalleeThunk<FunctionType, function>::Thunk, 0);
}

template <typename MethodType, MethodType method>
void ConcreteMethod<MethodType, method>::PushThunk(lua_State* luaState)
{
	lua_pushcclosure(luaState, &CalleeThunk<MethodType, method>::Thunk, 0);
}

template <typename ConstructorType>
void ConcreteConstructor<ConstructorType>::PushThunk(lua_State* luaState)
{
	lua_pushcclosure(luaState, &ConstructorThunk<ConstructorType>::Thunk, 0);
}

template <typename ClassType>
ConcreteClass<ClassType>::ConcreteClass(const char* fullName, void (*init)(ConcreteClass<ClassType>& cls)) : Class(fullName)
{
	if(init)
		init(*this);
}

template <typename CalleeType, CalleeType callee, int isMethod>
struct CalleeAdder;

template <typename CalleeType, CalleeType callee>
struct CalleeAdder<CalleeType, callee, 0>
{
	static void Add(Class* cls, const char* name)
	{
		cls->staticMethods.push_back(new ConcreteFunction<CalleeType, callee>(name));
	}
};

template <typename CalleeType, CalleeType callee>
struct CalleeAdder<CalleeType, callee, 1>
{
	static void Add(Class* cls, const char* name)
	{
		cls->methods.push_back(new ConcreteMethod<CalleeType, callee>(name));
	}
};

template <typename ConstructorType>
void Class::SetConstructor()
{
	constructor = new ConcreteConstructor<ConstructorType>();
}

template <typename MethodType, MethodType method>
void Class::AddMethod(const char* name)
{
	CalleeAdder<MethodType, method, Callable<MethodType>::isMethod>::Add(this, name);
}

END_INANITY_LUA

#endif
