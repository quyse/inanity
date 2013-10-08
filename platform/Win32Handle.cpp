#include "Win32Handle.hpp"

BEGIN_INANITY_PLATFORM

Win32Handle::Win32Handle(HANDLE handle) : handle(handle) {}

Win32Handle::~Win32Handle()
{
	if(IsValid())
		CloseHandle(handle);
}

void Win32Handle::operator=(HANDLE handle)
{
	this->handle = handle;
}

bool Win32Handle::IsValid() const
{
	return handle && handle != INVALID_HANDLE_VALUE;
}

Win32Handle::operator HANDLE()
{
	return handle;
}

END_INANITY_PLATFORM
