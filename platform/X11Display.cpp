#include "X11Display.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_PLATFORM

X11Display::X11Display(Display* display) : display(display) {}

X11Display::~X11Display()
{
	XCloseDisplay(display);
}

Display* X11Display::GetDisplay() const
{
	return display;
}

ptr<X11Display> X11Display::Open(const char* displayName)
{
	Display* display = XOpenDisplay(displayName);
	if(!display)
		THROW_PRIMARY_EXCEPTION("Can't open X11 display");

	return NEW(X11Display(display));
}

ptr<X11Display> X11Display::CreateDefault()
{
	return Open(NULL);
}

END_INANITY_PLATFORM
