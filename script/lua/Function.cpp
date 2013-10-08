#include "Function.hpp"
#include "Any.hpp"
#include "State.hpp"

BEGIN_INANITY_LUA

Function::Function(ptr<Any> function)
: function(function) {}

ptr<Script::Any> Function::Run()
{
	return function->Call();
}

END_INANITY_LUA
