#ifndef ___INANITY_SAFE_OBJECT_HPP___
#define ___INANITY_SAFE_OBJECT_HPP___

#include "Object.hpp"
#include <atomic>

BEGIN_INANITY

/// Base class for managed thread-safe objects.
/** Compatible with general "ptr" managed pointers. */
class SafeObject
{
private:
	std::atomic<int> referencesCount;

public:
	inline SafeObject() : referencesCount(0) {}

	virtual ~SafeObject() {}

	inline void Reference()
	{
		referencesCount++;
	}

	inline void Dereference()
	{
		if(!--referencesCount)
			delete this;
	}

	inline int GetReferencesCount() const
	{
		return referencesCount;
	}

	static void* operator new(size_t size);
	static void operator delete(void* data);
};

END_INANITY

#endif
