#ifndef ___INANITY_GRAPHICS_WIN32_OUTPUT_HPP___
#define ___INANITY_GRAPHICS_WIN32_OUTPUT_HPP___

#include "Output.hpp"
#include "../platform/platform.hpp"
#include "../windows.hpp"

BEGIN_INANITY_PLATFORM

class Win32Window;

END_INANITY_PLATFORM

BEGIN_INANITY_GRAPHICS

/// Win32 graphics output class.
/** Always a window, fullscreen or not. */
class Win32Output : public Output
{
private:
	ptr<Platform::Win32Window> window;

public:
	Win32Output(ptr<Platform::Win32Window> window);
	~Win32Output();

	//*** Output's methods.
	int GetWidth() const;
	int GetHeight() const;

	ptr<Platform::Win32Window> GetWindow() const;
	HWND GetHWND() const;
	void Resize(int width, int height);
};

END_INANITY_GRAPHICS

#endif
