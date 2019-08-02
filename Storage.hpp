#ifndef ___INANITY_STORAGE_HPP___
#define ___INANITY_STORAGE_HPP___

#include "Object.hpp"

BEGIN_INANITY

class InputStream;

class Storage : public Object
{
public:
	/// Get size of storage.
	virtual bigsize_t GetBigSize() const = 0;
	/// Read data from storage.
	/** Offset and size must be within total size.
	Default implementation calls GetInputStream. */
	virtual void Read(bigsize_t offset, size_t size, void* data);
	/// Get input stream for reading selected chunk.
	/** Default implementation calls Read. */
	virtual ptr<InputStream> GetInputStream(bigsize_t offset, bigsize_t size);
};

END_INANITY

#endif
