#ifndef ___INANITY_SCRIPT_V8_STATE_HPP___
#define ___INANITY_SCRIPT_V8_STATE_HPP___

#include "v8.hpp"
#include "../State.hpp"
#include "v8lib.hpp"

BEGIN_INANITY_V8

/// The state for V8 virtual machine.
class State : public Inanity::Script::State
{
private:
	v8::Isolate* isolate;
	v8::Persistent<v8::Context> context;

public:
	class Scope
	{
	private:
		State* state;

	public:
		Scope(State* state);
		~Scope();
	};

public:
	State();
	~State();

	v8::Isolate* GetIsolate() const;

	// Script::State's methods.
	void Register(Meta::ClassBase* classMeta);
	ptr<Script::Function> LoadScript(ptr<File> file);
};

END_INANITY_V8

#endif
