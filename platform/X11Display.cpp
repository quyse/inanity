#include "X11Display.hpp"
#include "../Exception.hpp"
#include <sstream>

BEGIN_INANITY_PLATFORM

X11Display::X11Display(::Display* display)
: display(display)
{
	connection = XGetXCBConnection(display);
}

X11Display::~X11Display()
{
	XCloseDisplay(display);
}

::Display* X11Display::GetDisplay() const
{
	return display;
}

xcb_connection_t* X11Display::GetConnection() const
{
	return connection;
}

void X11Display::ThrowError(xcb_generic_error_t* error)
{
	std::ostringstream s;
	s << "XCB error #" << error->error_code << ' ' << error->major_code << '.' << error->minor_code;
	XFree(error);
	THROW_PRIMARY_EXCEPTION(s.str());
}

ptr<X11Display> X11Display::Open(const char* displayName)
{
	::Display* display = XOpenDisplay(displayName);
	if(!display)
		THROW_PRIMARY_EXCEPTION("Can't open X11 display");

	return NEW(X11Display(display));
}

ptr<X11Display> X11Display::CreateDefault()
{
	return Open(NULL);
}

END_INANITY_PLATFORM
