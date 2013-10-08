#ifndef ___INANITY_PLATFORM_WIN32_HANDLE_HPP___
#define ___INANITY_PLATFORM_WIN32_HANDLE_HPP___

#include "platform.hpp"
#include "windows.hpp"

BEGIN_INANITY_PLATFORM

/// Class incapsulates Windows HANDLE.
class Win32Handle : public Object
{
private:
	HANDLE handle;

public:
	Win32Handle(HANDLE handle = 0);
	~Win32Handle();

	void operator=(HANDLE handle);
	bool IsValid() const;
	operator HANDLE();
};

END_INANITY_PLATFORM

#endif
