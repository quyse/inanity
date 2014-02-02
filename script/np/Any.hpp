#ifndef ___INANITY_SCRIPT_NP_ANY_HPP___
#define ___INANITY_SCRIPT_NP_ANY_HPP___

#include "np.hpp"
#include "../Any.hpp"
#include "../../platform/npapi.hpp"

BEGIN_INANITY_NP

class State;

/// Any value for NP scripting.
class Any : public Script::Any
{
private:
	ptr<State> state;
	NPVariant variant;

	ptr<Any> InternalGet(NPIdentifier identifier) const;
	void InternalSet(NPIdentifier identifier, ptr<Script::Any> value);

public:
	/// Create any value from NPVariant.
	/** If variant represents object, its reference count
	increases by one. If variant represents string,
	it doesn't copy, but in dtor it's freed, so it should
	be allocated with NPAPI allocation functions. */
	Any(ptr<State> state, NPVariant variant);
	~Any();

	ptr<State> GetState() const;
	NPVariant GetVariant() const;

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

END_INANITY_NP

#endif
