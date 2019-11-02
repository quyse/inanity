#ifndef ___INANITY_STORAGE_INPUT_STREAM_HPP___
#define ___INANITY_STORAGE_INPUT_STREAM_HPP___

#include "InputStream.hpp"

BEGIN_INANITY

class Storage;

/// Input stream, reading from Storage.
class StorageInputStream : public InputStream
{
private:
	ptr<Storage> storage;
	bigsize_t offset;
	bigsize_t endOffset;

public:
	StorageInputStream(ptr<Storage> storage, bigsize_t offset = 0);
	StorageInputStream(ptr<Storage> storage, bigsize_t offset, bigsize_t size);

	size_t Read(void* data, size_t size) override;
	bigsize_t Skip(bigsize_t size) override;
	bool IsAtEnd() const override;
};

END_INANITY

#endif
