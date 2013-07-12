#ifndef ___INANITY_PLATFORM_X11_DISPLAY_HPP___
#define ___INANITY_PLATFORM_X11_DISPLAY_HPP___

#include "platform.hpp"
#include "x11.hpp"

BEGIN_INANITY_PLATFORM

/// Класс, инкапсулирующий дисплей X11.
class X11Display : public Object
{
private:
	Display* display;

public:
	X11Display(Display* display);
	~X11Display();

	Display* GetDisplay() const;

	static ptr<X11Display> Open(const char* displayName);
	static ptr<X11Display> CreateDefault();
};

END_INANITY_PLATFORM

#endif
