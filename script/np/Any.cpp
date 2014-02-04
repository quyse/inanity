#include "Any.hpp"
#include "State.hpp"
#include "values.ipp"
#include "wrappers.hpp"
#include "../../platform/NpapiPlugin.hpp"
#include "../../platform/NpapiPluginInstance.hpp"
#include "../../Exception.hpp"

BEGIN_INANITY_NP

Any::Any(ptr<State> state, NPVariant variant)
: state(state), variant(variant) {}

Any::~Any()
{
	Platform::NpapiPlugin::browserFuncs.releasevariantvalue(&variant);
}

ptr<State> Any::GetState() const
{
	return state;
}

NPVariant Any::GetVariant() const
{
	if(NPVARIANT_IS_OBJECT(variant))
		Platform::NpapiPlugin::browserFuncs.retainobject(NPVARIANT_TO_OBJECT(variant));
	return variant;
}

bool Any::IsNull() const
{
	return NPVARIANT_IS_VOID(variant) || NPVARIANT_IS_NULL(variant);
}

bool Any::IsBoolean() const
{
	return NPVARIANT_IS_BOOLEAN(variant);
}

bool Any::IsNumber() const
{
	return NPVARIANT_IS_INT32(variant) || NPVARIANT_IS_DOUBLE(variant);
}

bool Any::IsString() const
{
	return NPVARIANT_IS_STRING(variant);
}

bool Any::IsArray() const
{
	return NPVARIANT_IS_OBJECT(variant);
}

bool Any::IsFunction() const
{
	// not precise
	return NPVARIANT_IS_OBJECT(variant);
}

bool Any::IsObject() const
{
	return NPVARIANT_IS_OBJECT(variant) && state->CheckClass(NPVARIANT_TO_OBJECT(variant)->_class);
}

ptr<RefCounted> Any::AsObject() const
{
	if(!IsObject())
		THROW("NP Any is not an Inanity object");
	return static_cast<NPObjectWrapper*>(NPVARIANT_TO_OBJECT(variant))->object;
}

bool Any::AsBool() const
{
	return Value<bool>::From(variant);
}

int Any::AsInt() const
{
	return Value<int>::From(variant);
}

float Any::AsFloat() const
{
	return Value<float>::From(variant);
}

double Any::AsDouble() const
{
	return Value<double>::From(variant);
}

String Any::AsString() const
{
	return Value<String>::From(variant);
}

ptr<Script::Any> Any::CallWith(ptr<Script::Any> arguments[], int count)
{
	// convert args
	NPVariant* args = (NPVariant*)alloca(sizeof(NPVariant) * count);
	for(int i = 0; i < count; ++i)
		args[i] = fast_cast<Any*>(&*arguments[i])->variant;

	// make a call
	NPVariant result;
	if(!Platform::NpapiPlugin::browserFuncs.invokeDefault(
		state->GetPluginInstance()->GetNpp(),
		NPVARIANT_TO_OBJECT(variant),
		args, count, &result))
		THROW("Can't call Np::Any as a function");

	return state->CreateAny(result);
}

ptr<Script::Any> Any::ApplyWith(ptr<Script::Any> thisValue, ptr<Script::Any> arguments[], int count)
{
	THROW("Np::Any doesn't support applying to");
}

ptr<Any> Any::InternalGet(NPIdentifier identifier) const
{
	NPVariant result;

	if(!Platform::NpapiPlugin::browserFuncs.getproperty(
		state->GetPluginInstance()->GetNpp(),
		NPVARIANT_TO_OBJECT(variant),
		identifier,
		&result))
		VOID_TO_NPVARIANT(result);

	return state->CreateAny(result);
}

int Any::GetLength() const
{
	return InternalGet(Platform::NpapiPlugin::browserFuncs.getstringidentifier("length"));
}

ptr<Script::Any> Any::Get(int index) const
{
	return InternalGet(Platform::NpapiPlugin::browserFuncs.getintidentifier(index));
}

ptr<Script::Any> Any::Get(ptr<Script::Any> index) const
{
	NPVariant indexVariant = fast_cast<Any*>(&*index)->variant;

	if(NPVARIANT_IS_STRING(indexVariant))
		return InternalGet(
			Platform::NpapiPlugin::browserFuncs.getstringidentifier(
				NPVARIANT_TO_STRING(indexVariant).UTF8Characters));

	if(NPVARIANT_IS_INT32(indexVariant))
		return Get(NPVARIANT_TO_INT32(indexVariant));

	NPVariant result;
	VOID_TO_NPVARIANT(result);
	return state->CreateAny(result);
}

void Any::InternalSet(NPIdentifier identifier, ptr<Script::Any> value)
{
	Platform::NpapiPlugin::browserFuncs.setproperty(
		state->GetPluginInstance()->GetNpp(),
		NPVARIANT_TO_OBJECT(variant),
		identifier,
		&fast_cast<Any*>(&*value)->variant);
}

void Any::Set(int index, ptr<Script::Any> value)
{
	InternalSet(
		Platform::NpapiPlugin::browserFuncs.getintidentifier(index),
		value);
}

void Any::Set(ptr<Script::Any> index, ptr<Script::Any> value)
{
	NPVariant indexVariant = fast_cast<Any*>(&*index)->variant;

	if(NPVARIANT_IS_STRING(indexVariant))
		InternalSet(
			Platform::NpapiPlugin::browserFuncs.getstringidentifier(
				NPVARIANT_TO_STRING(indexVariant).UTF8Characters),
			value);
	else if(NPVARIANT_IS_INT32(indexVariant))
		Set(NPVARIANT_TO_INT32(indexVariant), value);
}

void Any::Dump(std::ostream& stream) const
{
	// TODO
}

END_INANITY_NP
