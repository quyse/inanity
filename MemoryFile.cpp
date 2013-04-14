#include "MemoryFile.hpp"
#include <string.h>

BEGIN_INANITY

MemoryFile::MemoryFile(size_t size) : size(size)
{
	data = size ? new char[size] : 0;
}

MemoryFile::MemoryFile(void* data, size_t size) : data(data), size(size)
{
}

MemoryFile::~MemoryFile()
{
	if(data)
		delete [] (char*)data;
}

ptr<MemoryFile> MemoryFile::CreateViaCopy(const void* data, size_t size)
{
	ptr<MemoryFile> file = NEW(MemoryFile(size));
	memcpy(file->GetData(), data, size);
	return file;
}

void* MemoryFile::GetData() const
{
	return data;
}

size_t MemoryFile::GetSize() const
{
	return size;
}

END_INANITY
