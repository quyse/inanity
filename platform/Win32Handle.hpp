#ifndef ___INANITY_PLATFORM_WIN32_HANDLE_HPP___
#define ___INANITY_PLATFORM_WIN32_HANDLE_HPP___

#include "platform.hpp"
#include "windows.hpp"

BEGIN_INANITY_PLATFORM

/// Class incapsulates Windows HANDLE.
class Win32Handle
{
private:
	HANDLE handle = nullptr;

public:
	Win32Handle(HANDLE handle = nullptr);
	Win32Handle(Win32Handle&& handle);
	~Win32Handle();

	Win32Handle& operator=(Win32Handle&& handle);

	bool IsValid() const;
	operator HANDLE();
};

END_INANITY_PLATFORM

#endif
