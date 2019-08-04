#include "Win32Handle.hpp"

BEGIN_INANITY_PLATFORM

Win32Handle::Win32Handle(HANDLE handle) : handle(handle) {}

Win32Handle::Win32Handle(Win32Handle&& handle)
{
	std::swap(this->handle, handle.handle);
}

Win32Handle::~Win32Handle()
{
	if(IsValid())
		CloseHandle(handle);
}

Win32Handle& Win32Handle::operator=(Win32Handle&& handle)
{
	std::swap(this->handle, handle.handle);
	return *this;
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
