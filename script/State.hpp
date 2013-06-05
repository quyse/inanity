#ifndef ___INANITY_SCRIPT_STATE_HPP___
#define ___INANITY_SCRIPT_STATE_HPP___

#include "script.hpp"

BEGIN_INANITY_SCRIPT

/// Abstract class incapsulating the state of scripting engine.
/** Script entities (functions, objects, etc) can't be directly
exchanged between instances of State class. */
class State
{
public:
	/// Registers an information about class.
	template <typename ClassType>
	void RegisterClass()
	{
		Lua::RegisterCls(state, ClassType::scriptClass);
	}
};

END_INANITY_SCRIPT

#endif
