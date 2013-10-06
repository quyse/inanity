#include "Function.hpp"
#include "State.hpp"
#include "Any.hpp"

BEGIN_INANITY_V8

Function::Function(ptr<State> state, v8::Local<v8::Script> script) :
	state(state),
	script(state->GetIsolate(), script)
{}

Function::~Function()
{
	script.Reset();
}

ptr<Script::Any> Function::Run()
{
	State::Scope scope(state);

	v8::TryCatch tryCatch;

	v8::Local<v8::Script> script =
		v8::Local<v8::Script>::New(state->GetIsolate(), this->script);

	v8::Local<v8::Value> returnValue = script->Run();

	state->ProcessErrors(tryCatch);

	return state->CreateAny(returnValue);
}

END_INANITY_V8
