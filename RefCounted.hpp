#ifndef ___INANITY_REF_COUNTED_HPP___
#define ___INANITY_REF_COUNTED_HPP___

#include "ptr.hpp"

BEGIN_INANITY

/// A base class for reference-counted objects.
class RefCounted
{
private:
	int referencesCount;

protected:
	/// Free an instance run out of references.
	virtual void FreeAsNotReferenced() = 0;

public:
	inline RefCounted() : referencesCount(0) {}
	virtual ~RefCounted() {}

	inline int GetReferencesCount() const
	{
		return referencesCount;
	}

	inline void Reference()
	{
		++referencesCount;
	}

	inline void Dereference()
	{
		if(!--referencesCount)
			FreeAsNotReferenced();
	}
};

END_INANITY

#endif
