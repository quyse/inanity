#include "Window.hpp"

BEGIN_INANITY_PLATFORM

Window::Window() :
	presenter(nullptr),
	mouseLock(false),
	cursorVisible(true),
	sleepWhenInactive(false),
	running(false),
	preventUserClose(false)
{}

float Window::GetDPIScale() const
{
	return 1;
}

void Window::SetPresenter(Graphics::Presenter* presenter)
{
	this->presenter = presenter;
}

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

void Window::Stop()
{
	running = false;
}

void Window::SetPreventUserClose(bool preventUserClose)
{
	this->preventUserClose = preventUserClose;
}

END_INANITY_PLATFORM
