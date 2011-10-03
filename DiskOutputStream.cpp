#include "DiskOutputStream.hpp"
#include "Handle.hpp"
#include "Exception.hpp"

DiskOutputStream::DiskOutputStream(ptr<Handle> handle) : handle(handle)
{
}

#ifdef ___INANITY_WINDOWS

void DiskOutputStream::Write(const void *data, size_t size)
{
	if((DWORD)size != size)
		THROW_PRIMARY_EXCEPTION("So big write size is not supported");
	DWORD written;
	if(!WriteFile(*handle, data, (DWORD)size, &written, NULL) || written != size)
		THROW_PRIMARY_EXCEPTION("Disk write error");
}

#endif // ___INANITY_WINDOWS

#ifdef ___INANITY_LINUX

void DiskOutputStream::Write(const void *data, size_t size)
{
	const char* dataPtr = (const char*)data;
	while(size > 0)
	{
		ssize_t written = write(*handle, dataPtr, size);
		if(written < 0)
			THROW_PRIMARY_EXCEPTION("Disk write error");
		size -= written;
		dataPtr += written;
	}
}

#endif // ___INANITY_LINUX
