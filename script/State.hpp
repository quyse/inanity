#ifndef ___INANITY_SCRIPT_STATE_HPP___
#define ___INANITY_SCRIPT_STATE_HPP___

#include "script.hpp"
#include "../meta/Class.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_SCRIPT

class Function;

/// Abstract class incapsulating the state of scripting engine.
/** Script entities (functions, objects, etc) can't be directly
exchanged between instances of State class. */
class State : public Object
{
public:
	/// Registers an information about class.
	virtual void Register(Meta::ClassBase* classMeta) = 0;

	/// Helper method for registering class by type.
	template <typename ClassType>
	void Register()
	{
		Register(&ClassType::meta);
	}

	/// Loads a script from file.
	virtual ptr<Function> LoadScript(ptr<File> file) = 0;
};

END_INANITY_SCRIPT

#endif
