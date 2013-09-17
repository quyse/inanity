#include "Function.hpp"
#include "State.hpp"

BEGIN_INANITY_V8

Function::Function(ptr<State> state, v8::Local<v8::Script> script) :
	state(state),
	script(state->GetIsolate(), script)
{}

Function::~Function()
{
	script.Dispose();
}

void Function::Run()
{
	State::Scope scope(state);

	v8::Local<v8::Script> script =
		v8::Local<v8::Script>::New(state->GetIsolate(), this->script);
	script->Run();
}

END_INANITY_V8
