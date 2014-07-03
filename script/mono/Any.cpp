#include "Any.hpp"
#include "State.hpp"
#include "values.ipp"
#include <mono/metadata/appdomain.h>
#include <cstring>

BEGIN_INANITY_MONO

Any::Any(ptr<State> state, MonoObject* object)
: state(state), object(object)
{
	handle = object ? mono_gchandle_new(object, true) : 0;
}

Any::~Any()
{
	if(object)
		mono_gchandle_free(handle);
}

ptr<State> Any::GetState() const
{
	return state;
}

MonoObject* Any::GetObject() const
{
	return object;
}

bool Any::IsNull() const
{
	return !!object;
}

bool Any::IsBoolean() const
{
	return mono_object_isinst(object, mono_get_boolean_class());
}

bool Any::IsNumber() const
{
	decltype(&mono_get_byte_class) classes[] =
	{
		&mono_get_byte_class,
		&mono_get_sbyte_class,
		&mono_get_int16_class,
		&mono_get_int32_class,
		&mono_get_int64_class,
		&mono_get_uint16_class,
		&mono_get_uint32_class,
		&mono_get_uint64_class,
		&mono_get_intptr_class,
		&mono_get_uintptr_class,
		&mono_get_single_class,
		&mono_get_double_class
	};
	for(size_t i = 0; i < sizeof(classes) / sizeof(classes[0]); ++i)
		if(mono_object_isinst(object, classes[i]()))
			return true;
	return false;
}

bool Any::IsString() const
{
	return mono_object_isinst(object, mono_get_string_class());
}

bool Any::IsArray() const
{
	return mono_object_isinst(object, mono_get_array_class());
}

bool Any::IsFunction() const
{
	// not implemented yet
	return false;
}

bool Any::IsObject() const
{
	return State::instance->IsObjectWrapper(object);
}

ptr<RefCounted> Any::AsObject() const
{
	return State::instance->UnwrapObject(object);
}

bool Any::AsBool() const
{
	return *(MonoBoolean*)mono_object_unbox(object);
}

int Any::AsInt() const
{
	return *(int*)mono_object_unbox(object);
}

float Any::AsFloat() const
{
	return *(float*)mono_object_unbox(object);
}

double Any::AsDouble() const
{
	return *(double*)mono_object_unbox(object);
}

String Any::AsString() const
{
	if(!IsString())
		THROW("Any is not a string");
	return Value<String>::From((MonoString*)object);
}

ptr<Script::Any> Any::CallWith(ptr<Script::Any> arguments[], int count)
{
	THROW("Not implemented yet");
}

ptr<Script::Any> Any::ApplyWith(ptr<Script::Any> thisValue, ptr<Script::Any> arguments[], int count)
{
	THROW("Not implemented yet");
}

int Any::GetLength() const
{
	return (int)mono_array_length((MonoArray*)object);
}

ptr<Script::Any> Any::Get(int index) const
{
	MonoClass* elementClass = mono_class_get_element_class(mono_object_get_class(object));
	void* element = mono_array_addr_with_size(
		(MonoArray*)object,
		mono_class_array_element_size(elementClass),
		index);
	MonoObject* result;
	if(mono_type_is_reference(mono_class_get_type(elementClass)))
		result = *(MonoObject**)element;
	else
		result = mono_value_box(state->GetDomain(), elementClass, element);
	return state->CreateAny(result);
}

ptr<Script::Any> Any::Get(ptr<Script::Any> index) const
{
	return Get(index->AsInt());
}

void Any::Set(int index, ptr<Script::Any> value)
{
	MonoClass* elementClass = mono_class_get_element_class(mono_object_get_class(object));
	int32_t elementSize = mono_class_array_element_size(elementClass);
	void* element = mono_array_addr_with_size((MonoArray*)object, elementSize, index);
	MonoObject* valueObject = fast_cast<Any*>(&*value)->GetObject();
	if(mono_type_is_reference(mono_class_get_type(elementClass)))
		*(MonoObject**)element = valueObject;
	else
		memcpy(element, mono_object_unbox(valueObject), elementSize);
}

void Any::Set(ptr<Script::Any> index, ptr<Script::Any> value)
{
	Set(index->AsInt(), value);
}

void Any::Dump(std::ostream& stream) const
{
	THROW("Not implemented yet");
}

END_INANITY_MONO
