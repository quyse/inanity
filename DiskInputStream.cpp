#include "DiskInputStream.hpp"
#include "Handle.hpp"
#include "Exception.hpp"

DiskInputStream::DiskInputStream(ptr<Handle> handle) : handle(handle)
{
}

#ifdef ___INANITY_WINDOWS

size_t DiskInputStream::Read(void *data, size_t size)
{
	if((DWORD)size != size)
		THROW_PRIMARY_EXCEPTION(L"So big reading size is not supported");
	DWORD read;
	ReadFile(*handle, data, (DWORD)size, &read, NULL);
	return read;
}

size_t DiskInputStream::GetSize() const
{
	LARGE_INTEGER size;
	if(!GetFileSizeEx(*handle, &size))
		THROW_PRIMARY_EXCEPTION(L"Error getting size");
	size_t returnSize = (size_t)size.QuadPart;
	if(returnSize != size.QuadPart)
		THROW_PRIMARY_EXCEPTION(L"File is too big");
	return returnSize;
}

#endif // ___INANITY_WINDOWS

#ifdef ___INANITY_LINUX

#include <climits>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

size_t DiskInputStream::Read(void *data, size_t size)
{
	char* dataPtr = (char*)data;
	size_t resultSize = 0;
	while(size > 0)
	{
		ssize_t readSize = size;
		if(readSize > SSIZE_MAX)
			readSize = SSIZE_MAX;
		readSize = read(*handle, dataPtr, readSize);
		if(readSize < 0)
			THROW_PRIMARY_EXCEPTION("Disk read error");
		if(readSize == 0)
			break;
		resultSize += readSize;
		size -= readSize;
		dataPtr += readSize;
	}
	return resultSize;
}

size_t DiskInputStream::GetSize() const
{
	struct stat st;
	if(fstat(*handle, &st) != 0)
		THROW_PRIMARY_EXCEPTION("Error getting size");
	return st.st_size;
}

#endif // ___INANITY_LINUX
