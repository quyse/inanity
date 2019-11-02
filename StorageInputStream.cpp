#include "StorageInputStream.hpp"
#include "Storage.hpp"
#include <algorithm>

BEGIN_INANITY

StorageInputStream::StorageInputStream(ptr<Storage> storage, bigsize_t offset)
: storage(storage), offset(offset), endOffset(storage->GetBigSize()) {}

StorageInputStream::StorageInputStream(ptr<Storage> storage, bigsize_t offset, bigsize_t size)
: storage(storage), offset(offset), endOffset(std::min(offset + size, storage->GetBigSize())) {}

size_t StorageInputStream::Read(void* data, size_t size)
{
	bigsize_t maxSize = endOffset - offset;
	if(size > maxSize)
		size = (size_t)maxSize;
	storage->Read(offset, size, data);
	offset += size;
	return size;
}

bigsize_t StorageInputStream::Skip(bigsize_t size)
{
	bigsize_t maxSize = endOffset - offset;
	if(size > maxSize)
		size = maxSize;
	offset += size;
	return size;
}

bool StorageInputStream::IsAtEnd() const
{
	return offset >= endOffset;
}

END_INANITY
