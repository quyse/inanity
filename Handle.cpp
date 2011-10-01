#include "Handle.hpp"

#ifdef ___INANITY_WINDOWS

Handle::Handle(HANDLE handle) : handle(handle) {}
Handle::~Handle()
{
	if(IsValid())
		CloseHandle(handle);
}

void Handle::operator=(HANDLE handle)
{
	this->handle = handle;
}

bool Handle::IsValid() const
{
	return handle && handle != INVALID_HANDLE_VALUE;
}

Handle::operator HANDLE()
{
	return handle;
}

#endif // ___INANITY_WINDOWS

#ifdef ___INANITY_LINUX

#include <unistd.h>

Handle::Handle(int fd) : fd(fd) {}
Handle::~Handle()
{
	if(IsValid())
		close(fd);
}

void Handle::operator=(int fd)
{
	this->fd = fd;
}

bool Handle::IsValid() const
{
	return fd >= 0;
}

Handle::operator int()
{
	return fd;
}

#endif // ___INANITY_LINUX
