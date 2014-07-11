#ifndef ___INANITY_SCRIPT_MONO_THUNKS_IPP___
#define ___INANITY_SCRIPT_MONO_THUNKS_IPP___

#include "values.hpp"
#include <mono/metadata/object.h>

BEGIN_INANITY_MONO

#define MONO_TRY_BEGIN() try {
#define MONO_TRY_END() } catch(Exception* exception) { State::instance->RaiseException(exception); }

//*** class ConstructorHelper

template <typename ClassType, typename... Args>
struct ConstructorHelper
{
	static void Thunk(MonoObject* thisObject, typename Value<Args>::MonoType... args)
	{
		MONO_TRY_BEGIN();

		ptr<ClassType> object = NEW(ClassType(Value<Args>::From(args)...));
		State::instance->SetObjectIntoWrapper(thisObject, &*object);

		MONO_TRY_END();
	}
};

//*** class FunctionHelper

// ReturnType (*)(Args...)
template <typename ReturnType, typename... Args, ReturnType (*function)(Args...)>
struct FunctionHelper<ReturnType (*)(Args...), function>
{
	static typename Value<ReturnType>::MonoType Thunk(typename Value<Args>::MonoType... args)
	{
		MONO_TRY_BEGIN();
		return Value<ReturnType>::To(function(Value<Args>::From(args)...));
		MONO_TRY_END();

		// no control could be here, it's only for compiler
		return typename Value<ReturnType>::MonoType();
	}
};

// void (*)(Args...)
template <typename... Args, void (*function)(Args...)>
struct FunctionHelper<void (*)(Args...), function>
{
	static void Thunk(typename Value<Args>::MonoType... args)
	{
		MONO_TRY_BEGIN();
		function(Value<Args>::From(args)...);
		MONO_TRY_END();
	}
};

//*** class MethodHelper

// ReturnType (ClassType::*)(Args...)
template <typename ReturnType, typename ClassType, typename... Args, ReturnType (ClassType::*method)(Args...)>
struct MethodHelper<ReturnType (ClassType::*)(Args...), method>
{
	static typename Value<ReturnType>::MonoType Thunk(MonoObject* thisObject, typename Value<Args>::MonoType... args)
	{
		MONO_TRY_BEGIN();
		return Value<ReturnType>::To((Value<ptr<ClassType> >::From(thisObject)->*method)(Value<Args>::From(args)...));
		MONO_TRY_END();

		// no control could be here, it's only for compiler
		return typename Value<ReturnType>::MonoType();
	}
};

// void (ClassType::*)(Args...)
template <typename ClassType, typename... Args, void (ClassType::*method)(Args...)>
struct MethodHelper<void (ClassType::*)(Args...), method>
{
	static void Thunk(MonoObject* thisObject, typename Value<Args>::MonoType... args)
	{
		MONO_TRY_BEGIN();
		(Value<ptr<ClassType> >::From(thisObject)->*method)(Value<Args>::From(args)...);
		MONO_TRY_END();
	}
};

// ReturnType (ClassType::*)(Args...) const
template <typename ReturnType, typename ClassType, typename... Args, ReturnType (ClassType::*method)(Args...) const>
struct MethodHelper<ReturnType (ClassType::*)(Args...) const, method>
{
	static typename Value<ReturnType>::MonoType Thunk(MonoObject* thisObject, typename Value<Args>::MonoType... args)
	{
		MONO_TRY_BEGIN();
		return Value<ReturnType>::To((Value<ptr<ClassType> >::From(thisObject)->*method)(Value<Args>::From(args)...));
		MONO_TRY_END();

		// no control could be here, it's only for compiler
		return typename Value<ReturnType>::MonoType();
	}
};

// void (ClassType::*)(Args...) const
template <typename ClassType, typename... Args, void (ClassType::*method)(Args...) const>
struct MethodHelper<void (ClassType::*)(Args...) const, method>
{
	static void Thunk(MonoObject* thisObject, typename Value<Args>::MonoType... args)
	{
		MONO_TRY_BEGIN();
		(Value<ptr<ClassType> >::From(thisObject)->*method)(Value<Args>::From(args)...);
		MONO_TRY_END();
	}
};

#undef MONO_TRY_BEGIN
#undef MONO_TRY_END

END_INANITY_MONO

#endif
