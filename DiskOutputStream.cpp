#include "DiskOutputStream.hpp"
#include "Handle.hpp"
#include "Exception.hpp"

BEGIN_INANITY

DiskOutputStream::DiskOutputStream(ptr<Handle> handle) : handle(handle)
{
}

END_INANITY

#ifdef ___INANITY_WINDOWS

#include "windows.hpp"

BEGIN_INANITY

void DiskOutputStream::Write(const void *data, size_t size)
{
	if((DWORD)size != size)
		THROW("So big write size is not supported");
	DWORD written;
	if(!::WriteFile(*handle, data, (DWORD)size, &written, NULL) || written != size)
		THROW("Disk write error");
}

END_INANITY

#endif // ___INANITY_WINDOWS

#ifdef ___INANITY_LINUX

#include <unistd.h>

BEGIN_INANITY

void DiskOutputStream::Write(const void *data, size_t size)
{
	const char* dataPtr = (const char*)data;
	while(size > 0)
	{
		ssize_t written = write(*handle, dataPtr, size);
		if(written < 0)
			THROW("Disk write error");
		size -= written;
		dataPtr += written;
	}
}

END_INANITY

#endif // ___INANITY_LINUX
