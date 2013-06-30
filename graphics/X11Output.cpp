#include "X11Output.hpp"
#include "../X11Window.hpp"

BEGIN_INANITY_GRAPHICS

X11Output::X11Output(ptr<X11Window> window) : window(window) {}

ptr<X11Window> X11Output::GetWindow() const
{
	return window;
}

END_INANITY_GRAPHICS
