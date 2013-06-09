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

ptr<X11Display> X11Display::CreateDefault()
{
	// открыть дисплей X11
	Display* display = XOpenDisplay(NULL);
	if(!display)
		THROW_PRIMARY_EXCEPTION("Can't open default X11 display");

	return NEW(X11Display(display));
}

END_INANITY_PLATFORM
