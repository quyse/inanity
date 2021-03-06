#ifndef ___INANITY_SCRIPT_ANY_HPP___
#define ___INANITY_SCRIPT_ANY_HPP___

#include "script.hpp"
#include "../PoolObject.hpp"
#include "../String.hpp"
#include <iostream>

BEGIN_INANITY_SCRIPT

/// Abstract class represents any script value.
class Any : public PoolObject
{
public:
	//*** Is-type methods.
	virtual bool IsNull() const = 0;
	virtual bool IsBoolean() const = 0;
	virtual bool IsNumber() const = 0;
	virtual bool IsString() const = 0;
	/// Gets if this is an array or script object.
	virtual bool IsArray() const = 0;
	virtual bool IsFunction() const = 0;
	/// Gets if this is a C++ object.
	virtual bool IsObject() const = 0;

	//*** Convert to a C++ object.
	virtual ptr<RefCounted> AsObject() const = 0;

	//*** Number conversion methods.
	/** Throw exception if it is not a number. */
	virtual bool AsBool() const = 0;
	virtual int AsInt() const = 0;
	virtual float AsFloat() const = 0;
	virtual double AsDouble() const = 0;
	virtual String AsString() const = 0;
	//*** Helper conversion operators.
	operator bool() const
	{
		return AsBool();
	}
	operator int() const
	{
		return AsInt();
	}
	operator float() const
	{
		return AsFloat();
	}
	operator double() const
	{
		return AsDouble();
	}
	operator String() const
	{
		return AsString();
	}

	//*** Function methods.
	/** Throw exception if this is not a function. */
	virtual ptr<Any> CallWith(ptr<Any> arguments[], int count) = 0;
	virtual ptr<Any> ApplyWith(ptr<Any> thisValue, ptr<Any> arguments[], int count) = 0;
	//*** Helper call methods.
	template <typename... Args>
	ptr<Any> Call(Args... args)
	{
		ptr<Any> arguments[] = { 0, args... };
		return CallWith(arguments + 1, sizeof(arguments) / sizeof(arguments[0]) - 1);
	}
	template <typename... Args>
	ptr<Any> Apply(ptr<Any> thisValue, Args... args)
	{
		ptr<Any> arguments[] = { 0, args... };
		return ApplyWith(thisValue, arguments + 1, sizeof(arguments) / sizeof(arguments[0]) - 1);
	}

	//*** Methods for arrays.

	/** Throw exception if this is not an array. */
	/// Get length of an array.
	/** Depending on script, length may not include all elements,
	but only "integer-indexed" or "sequentially indexed". */
	virtual int GetLength() const = 0;

	/// Gets an element by numerical index.
	/** Index always 0-based, even in scripts with 1-based arrays.
	So index should be in [0, length). This method returns
	elements from "sequential" part of an array,
	but maybe not return elements from "hash" part.
	To get any element by real index without rebasing,
	use "any" version of Get method. */
	virtual ptr<Any> Get(int index) const = 0;

	/// Gets an element by any index.
	/** No conversions/rebasing are done on index. */
	virtual ptr<Any> Get(ptr<Any> index) const = 0;

	/// Sets an element by numerical index.
	/** Same rules applied as with Get method.
	See comments above. */
	virtual void Set(int index, ptr<Any> value) = 0;

	/// Sets an element by any index.
	virtual void Set(ptr<Any> index, ptr<Any> value) = 0;

	//*** Miscellaneous.

	/// Dumps text representation of a value in a script-specific way.
	virtual void Dump(std::ostream& stream) const = 0;
};

END_INANITY_SCRIPT

#endif
