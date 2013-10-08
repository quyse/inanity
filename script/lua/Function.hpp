#ifndef ___INANITY_SCRIPT_LUA_FUNCTION_HPP___
#define ___INANITY_SCRIPT_LUA_FUNCTION_HPP___

#include "lua.hpp"
#include "../Function.hpp"

BEGIN_INANITY_LUA

class Any;

/// Lua function class.
class Function : public Script::Function
{
private:
	ptr<Any> function;

public:
	Function(ptr<Any> function);

	//*** Script::Function method.
	ptr<Script::Any> Run();
};

END_INANITY_LUA

#endif
