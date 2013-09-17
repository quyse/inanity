#ifndef ___INANITY_SCRIPT_V8_FUNCTION_HPP___
#define ___INANITY_SCRIPT_V8_FUNCTION_HPP___

#include "v8.hpp"
#include "../Function.hpp"
#include "v8lib.hpp"

BEGIN_INANITY_V8

class State;

/// V8 function class.
class Function : public Script::Function
{
private:
	ptr<State> state;
	v8::Persistent<v8::Script> script;

public:
	Function(ptr<State> state, v8::Local<v8::Script> script);
	~Function();

	//*** Script::Function methods
	void Run();
};

END_INANITY_V8

#endif
