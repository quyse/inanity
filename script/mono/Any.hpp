#ifndef ___INANITY_SCRIPT_MONO_ANY_HPP___
#define ___INANITY_SCRIPT_MONO_ANY_HPP___

#include "mono.hpp"
#include "../Any.hpp"
#include <mono/metadata/object.h>

BEGIN_INANITY_MONO

class State;

class Any : public Script::Any
{
private:
	ptr<State> state;
	MonoObject* object;
	uint32_t handle;

public:
	Any(ptr<State> state, MonoObject* object);
	~Any();

	ptr<State> GetState() const;
	MonoObject* GetObject() const;

	//*** Script::Any's methods.
	bool IsNull() const;
	bool IsBoolean() const;
	bool IsNumber() const;
	bool IsString() const;
	bool IsArray() const;
	bool IsFunction() const;
	bool IsObject() const;
	ptr<RefCounted> AsObject() const;
	bool AsBool() const;
	int AsInt() const;
	float AsFloat() const;
	double AsDouble() const;
	String AsString() const;
	ptr<Script::Any> CallWith(ptr<Script::Any> arguments[], int count);
	ptr<Script::Any> ApplyWith(ptr<Script::Any> thisValue, ptr<Script::Any> arguments[], int count);
	int GetLength() const;
	ptr<Script::Any> Get(int index) const;
	ptr<Script::Any> Get(ptr<Script::Any> index) const;
	void Set(int index, ptr<Script::Any> value);
	void Set(ptr<Script::Any> index, ptr<Script::Any> value);
	void Dump(std::ostream& stream) const;
};

END_INANITY_MONO

#endif
