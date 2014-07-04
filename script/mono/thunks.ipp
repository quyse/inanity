#ifndef ___INANITY_SCRIPT_MONO_THUNKS_IPP___
#define ___INANITY_SCRIPT_MONO_THUNKS_IPP___

#include "values.hpp"
#include <mono/metadata/object.h>

BEGIN_INANITY_MONO

//*** class ConstructorHelper

template <typename ClassType, typename... Args>
struct ConstructorHelper
{
	static void Thunk(MonoObject* thisObject, typename Value<Args>::MonoType... args)
	{
		ptr<ClassType> object = NEW(ClassType(args...));
		State::instance->SetObjectIntoWrapper(thisObject, &*object);
	}
};

//*** class FunctionHelper

// ReturnType (*)(Args...)
template <typename ReturnType, typename... Args, ReturnType (*function)(Args...)>
struct FunctionHelper<ReturnType (*)(Args...), function>
{
	static typename Value<ReturnType>::MonoType Thunk(typename Value<Args>::MonoType... args)
	{
		return Value<ReturnType>::To(function(Value<Args>::From(args)...));
	}
};

// void (*)(Args...)
template <typename... Args, void (*function)(Args...)>
struct FunctionHelper<void (*)(Args...), function>
{
	static void Thunk(typename Value<Args>::MonoType... args)
	{
		function(Value<Args>::From(args)...);
	}
};

//*** class MethodHelper

// ReturnType (ClassType::*)(Args...)
template <typename ReturnType, typename ClassType, typename... Args, ReturnType (ClassType::*method)(Args...)>
struct MethodHelper<ReturnType (ClassType::*)(Args...), method>
{
	static typename Value<ReturnType>::MonoType Thunk(MonoObject* thisObject, typename Value<Args>::MonoType... args)
	{
		return Value<ReturnType>::To((Value<ptr<ClassType> >::From(thisObject)->*method)(Value<Args>::From(args)...));
	}
};

// void (ClassType::*)(Args...)
template <typename ClassType, typename... Args, void (ClassType::*method)(Args...)>
struct MethodHelper<void (ClassType::*)(Args...), method>
{
	static void Thunk(MonoObject* thisObject, typename Value<Args>::MonoType... args)
	{
		(Value<ptr<ClassType> >::From(thisObject)->*method)(Value<Args>::From(args)...);
	}
};

// ReturnType (ClassType::*)(Args...) const
template <typename ReturnType, typename ClassType, typename... Args, ReturnType (ClassType::*method)(Args...) const>
struct MethodHelper<ReturnType (ClassType::*)(Args...) const, method>
{
	static typename Value<ReturnType>::MonoType Thunk(MonoObject* thisObject, typename Value<Args>::MonoType... args)
	{
		return Value<ReturnType>::To((Value<ptr<ClassType> >::From(thisObject)->*method)(Value<Args>::From(args)...));
	}
};

// void (ClassType::*)(Args...) const
template <typename ClassType, typename... Args, void (ClassType::*method)(Args...) const>
struct MethodHelper<void (ClassType::*)(Args...) const, method>
{
	static void Thunk(MonoObject* thisObject, typename Value<Args>::MonoType... args)
	{
		(Value<ptr<ClassType> >::From(thisObject)->*method)(Value<Args>::From(args)...);
	}
};

END_INANITY_MONO

#endif
