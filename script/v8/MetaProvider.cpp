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

END_INANITY_V8
