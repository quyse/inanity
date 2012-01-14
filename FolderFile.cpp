#include "FolderFile.hpp"

#ifdef ___INANITY_WINDOWS
#include "windows.hpp"
#endif
#ifdef ___INANITY_LINUX
#include <sys/mman.h>
#endif

FolderFile::FolderFile(void* data, size_t size) : data(data), size(size)
{
}

FolderFile::~FolderFile()
{
#ifdef ___INANITY_WINDOWS
	UnmapViewOfFile(data);
#endif
#ifdef ___INANITY_LINUX
	munmap(data, size);
#endif
}

void* FolderFile::GetData() const
{
	return data;
}

size_t FolderFile::GetSize() const
{
	return size;
}
