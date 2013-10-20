#include "Window.hpp"

BEGIN_INANITY_PLATFORM

Window::Window() : mouseLock(false), cursorVisible(true) {}

void Window::SetMouseLock(bool mouseLock)
{
	if(this->mouseLock != mouseLock)
	{
		this->mouseLock = mouseLock;
		UpdateMouseLock();
	}
}

void Window::SetCursorVisible(bool cursorVisible)
{
	if(this->cursorVisible != cursorVisible)
	{
		this->cursorVisible = cursorVisible;
		UpdateCursorVisible();
	}
}

END_INANITY_PLATFORM
