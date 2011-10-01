#include "PartFile.hpp"

PartFile::PartFile(ptr<File> file, void* data, size_t size) : file(file), data(data), size(size)
{
}

void* PartFile::GetData() const
{
	return data;
}

size_t PartFile::GetSize() const
{
	return size;
}
