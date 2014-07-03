#include "MetaProvider.hpp"
#include "State.hpp"
#include "values.hpp"
#include "ClassBase.hpp"
#include <mono/metadata/object.h>

BEGIN_INANITY_MONO

//*** class MetaProvider::Constructor

template <typename ClassType, typename... Args>
class MetaProvider::Constructor : public MetaProvider::ConstructorBase
{
private:
	static void Thunk(MonoObject* thisObject, typename Value<Args>::MonoType... args)
	{
		ptr<ClassType> object = NEW(ClassType(args...));
		State::instance->SetObjectIntoWrapper(thisObject, &*object);
	}

public:
	void RegisterThunk()
	{
		mono_add_internal_call(GetInternalCallName().c_str(), (const void*)&Thunk);
	}
};

//*** helper class FunctionHelper

template <typename FunctionType, FunctionType function>
struct FunctionHelper;

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

//*** class MetaProvider::Function

template <typename FunctionType, FunctionType function>
class MetaProvider::Function : public MetaProvider::FunctionBase
{
public:
	Function(const char* name) : FunctionBase(name) {}

	void RegisterThunk()
	{
		mono_add_internal_call(GetInternalCallName().c_str(), (const void*)&FunctionHelper<FunctionType, function>::Thunk);
	}
};

//*** helper class MethodHelper

template <typename MethodType, MethodType method>
struct MethodHelper;

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

//*** class MetaProvider::Method

template <typename MethodType, MethodType method>
class MetaProvider::Method : public MetaProvider::MethodBase
{
public:
	Method(const char* name) : MethodBase(name) {}

	void RegisterThunk()
	{
		mono_add_internal_call(GetInternalCallName().c_str(), (const void*)&MethodHelper<MethodType, method>::Thunk);
	}
};

//*** class MetaProvider::Class

template <typename ClassType>
class MetaProvider::Class : public MetaProvider::ClassBase
{
public:
	Class(const char* name, const char* fullName)
	: ClassBase(name, fullName) {}
};

END_INANITY_MONO
