#ifndef ___INANITY_PLATFORM_X11_DISPLAY_HPP___
#define ___INANITY_PLATFORM_X11_DISPLAY_HPP___

#include "platform.hpp"
#include "x11.hpp"

BEGIN_INANITY_PLATFORM

/// Class incapsulating X11 display.
class X11Display : public Object
{
private:
	::Display* display;
	xcb_connection_t* connection;

public:
	X11Display(::Display* display);
	~X11Display();

	::Display* GetDisplay() const;
	xcb_connection_t* GetConnection() const;

	void ThrowError(xcb_generic_error_t* error);

	static ptr<X11Display> Open(const char* displayName);
	static ptr<X11Display> CreateDefault();
};

END_INANITY_PLATFORM

#endif
