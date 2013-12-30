#ifndef ___INANITY_SCRIPT_STATE_HPP___
#define ___INANITY_SCRIPT_STATE_HPP___

#include "convert.hpp"
#include "../String.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_SCRIPT

class Function;
class Any;

/// Abstract class incapsulating the state of scripting engine.
/** Script entities (functions, objects, etc) can't be directly
exchanged between instances of State class. */
class State : public Object
{
public:
	/// Loads a script from file.
	virtual ptr<Function> LoadScript(ptr<File> file) = 0;

	/// Unregister instance of object if exposed to script.
	/** Invalidates object instances in script (if they exist), and releases a reference.
	Invalidated instances should not be touched by script ever. */
	virtual void ReclaimInstance(RefCounted* object) = 0;

	//*** Creation script values.
	virtual ptr<Any> NewBoolean(bool boolean) = 0;
	virtual ptr<Any> NewNumber(int number) = 0;
	virtual ptr<Any> NewNumber(float number) = 0;
	virtual ptr<Any> NewNumber(double number) = 0;
	virtual ptr<Any> NewString(const String& string) = 0;
	virtual ptr<Any> NewArray(int length = 0) = 0;
	virtual ptr<Any> NewDict() = 0;

	/// Convert value-type to script value.
	template <typename T>
	ptr<Any> ConvertValue(const T& value)
	{
		return ConvertToScript<T>(this, value);
	}
};

END_INANITY_SCRIPT

#endif
