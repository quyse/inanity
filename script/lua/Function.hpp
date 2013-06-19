#ifndef ___INANITY_SCRIPT_LUA_FUNCTION_HPP___
#define ___INANITY_SCRIPT_LUA_FUNCTION_HPP___

#include "lua.hpp"
#include "../Function.hpp"

BEGIN_INANITY_LUA

class State;

/// Lua function class.
/** Stores function in Lua Registry by address of the object. */
class Function : public Script::Function
{
private:
	ptr<State> state;

public:
	/// Create Lua function.
	/** Pops function from top of the stack. */
	Function(ptr<State> state);
	~Function();

	//*** Script::Function methods
	void Run();
};

END_INANITY_LUA

#endif
