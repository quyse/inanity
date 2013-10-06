#ifndef ___INANITY_SCRIPT_V8_ANY_HPP___
#define ___INANITY_SCRIPT_V8_ANY_HPP___

#include "v8.hpp"
#include "../Any.hpp"
#include "v8lib.hpp"

BEGIN_INANITY_V8

class State;

class Any : public Script::Any
{
private:
	ptr<State> state;
	v8::Persistent<v8::Value> value;

public:
	Any(ptr<State> state, v8::Local<v8::Value> value);
	~Any();

	v8::Local<v8::Value> GetV8Value() const;

	//*** Script::Any's methods.
	bool IsNull() const;
	bool IsNumber() const;
	bool IsArray() const;
	bool IsFunction() const;
	bool IsObject() const;
	ptr<RefCounted> AsObject() const;
	int AsInt() const;
	float AsFloat() const;
	double AsDouble() const;
	ptr<Script::Any> CallWith(ptr<Script::Any>* arguments, int count);
	ptr<Script::Any> ApplyWith(ptr<Script::Any> thisValue, ptr<Script::Any>* arguments, int count);
	ptr<Script::Any> Get(int index) const;
	ptr<Script::Any> Get(ptr<Script::Any> index) const;
	void Set(int index, ptr<Script::Any> value);
	void Set(ptr<Script::Any> index, ptr<Script::Any> value);
};

END_INANITY_V8

#endif
