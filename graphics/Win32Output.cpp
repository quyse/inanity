#include "Win32Output.hpp"
#include "../platform/Win32Window.hpp"

BEGIN_INANITY_GRAPHICS

Win32Output::Win32Output(ptr<Platform::Win32Window> window) : window(window)
{
}

int Win32Output::GetWidth() const
{
	return window->GetClientWidth();
}

int Win32Output::GetHeight() const
{
	return window->GetClientHeight();
}

ptr<Platform::Win32Window> Win32Output::GetWindow() const
{
	return window;
}

HWND Win32Output::GetHWND() const
{
	return window->GetHWND();
}

END_INANITY_GRAPHICS
