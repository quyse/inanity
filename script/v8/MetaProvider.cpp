#include "MetaProvider.ipp"

BEGIN_INANITY_V8

MetaProvider::Any::Any(v8::Local<v8::Value> value)
: value(value) {}

MetaProvider::Any::operator float() const
{
	return (float)value->NumberValue();
}

MetaProvider::Any::operator double() const
{
	return (double)value->NumberValue();
}

MetaProvider::Any MetaProvider::Any::operator[](int i) const
{
	return v8::Object::Cast(*value)->Get(i);
}

v8::Local<v8::Value> MetaProvider::ReturnAny::ToValue() const
{
	return value;
}

void MetaProvider::ReturnAny::SetFloatArray(const float* floats, int count)
{
	v8::Local<v8::Array> array = v8::Array::New(count);
	value = array;
	for(int i = 0; i < count; ++i)
		array->Set(i, v8::NumberObject::New(floats[i]));
}

END_INANITY_V8
