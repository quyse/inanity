#ifndef ___INANITY_SCRIPT_MONO_INTEROP_IPP___
#define ___INANITY_SCRIPT_MONO_INTEROP_IPP___

#include "interop.hpp"
#include "../../meta/Callable.ipp"
#include "../../Exception.hpp"
#include <mono/metadata/debug-helpers.h>
#include <sstream>

BEGIN_INANITY_MONO

//*** helper class InteropHelper

template <typename T>
struct InteropHelper
{
	typedef T MonoType;
	static T To(T t) { return t; }
	static void* ToParam(T& t) { return &t; }
	static T From(T t) { return t; }
};
template <>
struct InteropHelper<ptr<DotNetObject> >
{
	typedef MonoObject* MonoType;
	static MonoObject* To(ptr<DotNetObject> object) { return object ? object->GetObject() : nullptr; }
	static void* ToParam(ptr<DotNetObject> object) { return To(object); }
	static ptr<DotNetObject> From(MonoObject* object) { return NEW(DotNetObject(object)); }
};
template <>
struct InteropHelper<ptr<DotNetArray> >
{
	typedef MonoArray* MonoType;
	static MonoArray* To(ptr<DotNetArray> array) { return array ? array->GetArray() : nullptr; }
	static void* ToParam(ptr<DotNetArray> array) { return To(array); }
	static ptr<DotNetArray> From(MonoArray* array) { return NEW(DotNetArray(array)); }
};

//*** class DotNetClass

template <typename... Args>
static void DoNothing(Args&&... args) {}

template <typename Seq, typename Type, typename... Args>
struct InitArrayHelper;
template <int... N, typename Type, typename... Args>
struct InitArrayHelper<Meta::TupleSeq<N...>, Type, Args...>
{
	static void InitArray(ptr<DotNetArray> array, Args... args)
	{
		DoNothing((array->Set<Type>(N, args), 0)...);
	}
};

template <typename Type, typename... Args>
ptr<DotNetArray> DotNetClass::CreateArray(ptr<DotNetDomain> domain, Args... args)
{
	ptr<DotNetArray> array = CreateArray(domain, sizeof...(Args));
	InitArrayHelper<typename Meta::TupleSeqGen<sizeof...(Args)>::Type, Type, Args...>::InitArray(array, args...);
	return array;
}

template <typename Type>
ptr<DotNetObject> DotNetClass::Box(ptr<DotNetDomain> domain, Type data)
{
	return NEW(DotNetObject(mono_value_box(domain->GetDomain(), klass, &data)));
}

//*** class DotNetObject

template <typename Type>
Type DotNetObject::Unbox()
{
	return InteropHelper<Type>::From((typename InteropHelper<Type>::MonoType*)mono_object_unbox(object));
}

//*** class DotNetArray

template <typename Type>
Type DotNetArray::Get(int index)
{
	return InteropHelper<Type>::From(mono_array_get(GetArray(), typename InteropHelper<Type>::MonoType, index));
}

template <typename Type>
void DotNetArray::Set(int index, Type data)
{
	mono_array_set(GetArray(), typename InteropHelper<Type>::MonoType, index, InteropHelper<Type>::To(data));
}

//*** class DotNetField

template <typename Type>
Type DotNetField::GetStatic(ptr<DotNetDomain> domain)
{
	typename InteropHelper<Type>::MonoType value;
	mono_field_static_get_value(
		mono_class_vtable(domain->GetDomain(), klass->GetClass()),
		field, &value);
	return InteropHelper<Type>::From(value);
}

template <typename Type>
void DotNetField::SetStatic(ptr<DotNetDomain> domain, Type data)
{
	mono_field_static_set_value(
		mono_class_vtable(domain->GetDomain(), klass->GetClass()),
		field, &InteropHelper<Type>::To(data));
}

template <typename Type>
Type DotNetField::Get(ptr<DotNetObject> thisObject)
{
	typename InteropHelper<Type>::MonoType value;
	mono_field_get_value(thisObject->GetObject(), field, &value);
	return InteropHelper<Type>::From(value);
}

template <typename Type>
void DotNetField::Set(ptr<DotNetObject> thisObject, Type data)
{
	mono_field_set_value(thisObject->GetObject(), field, &InteropHelper<Type>::To(data));
}

//*** class DotNetMethod

template <typename... Args>
MonoObject* DotNetMethod::CallHelper(MonoMethod* method, MonoObject* thisObject, Args... args, MonoObject** exception)
{
	void* params[] = { nullptr, InteropHelper<Args>::ToParam(args)... };
	return mono_runtime_invoke(method, thisObject, params + 1, exception);
}

template <typename... Args>
ptr<DotNetObject> DotNetMethod::Call(ptr<DotNetObject> thisObject, Args... args)
{
	MonoObject* exception = nullptr;
	MonoObject* result = CallHelper<Args...>(method, thisObject ? thisObject->GetObject() : nullptr, args..., &exception);
	if(exception)
	{
		std::ostringstream stream;
		stream << "Call method " << mono_method_full_name(method, true) << ": .NET exception of type " << mono_class_get_name(mono_object_get_class(exception));
		ptr<DotNetObject> e = NEW(DotNetObject(exception));
		ptr<DotNetObject> message = e->GetClass()->GetProperty("Message")->GetGetter()->Call(e);
		stream << ", message: " << mono_string_to_utf8((MonoString*)message->GetObject());
		THROW(stream.str());
	}
	return NEW(DotNetObject(result));
}

//*** class DotNetConstructor

template <typename... Args>
ptr<DotNetObject> DotNetConstructor::Construct(ptr<DotNetDomain> domain, Args... args)
{
	ptr<DotNetObject> object = klass->CreateObject(domain);
	method->Call(object, args...);
	return object;
}

END_INANITY_MONO

#endif
