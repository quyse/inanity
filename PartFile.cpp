#include "PartFile.hpp"

BEGIN_INANITY

PartFile::PartFile(ptr<File> file, void* data, size_t size)
: file(file), data(data), size(size) {}

PartFile::PartFile(ptr<File> file, size_t offset, size_t size)
: file(file), size(size)
{
	data = (char*)file->GetData() + offset;
}

void* PartFile::GetData() const
{
	return data;
}

size_t PartFile::GetSize() const
{
	return size;
}

END_INANITY
