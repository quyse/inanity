#include "Win32Output.hpp"
#include "../Win32Window.hpp"

Win32Output::Win32Output(ptr<Win32Window> window) : window(window)
{
}

HWND Win32Output::GetHWND() const
{
	return window->GetHWND();
}
