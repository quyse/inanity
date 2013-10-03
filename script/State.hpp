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
		Register(ClassType::GetMeta());
	}

	/// Loads a script from file.
	virtual ptr<Function> LoadScript(ptr<File> file) = 0;

	/// Unregister instance of object if exposed to script.
	/** Invalidates object instances in script (if they exist), and releases a reference.
	Invalidated instances should not be touched by script ever. */
	virtual void ReclaimInstance(RefCounted* object) = 0;
};

END_INANITY_SCRIPT

#endif
