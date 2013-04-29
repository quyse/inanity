#ifndef ___INANITY_GRAPHICS_X11_OUTPUT_HPP___
#define ___INANITY_GRAPHICS_X11_OUTPUT_HPP___

#include "Output.hpp"

BEGIN_INANITY

class X11Window;

END_INANITY

BEGIN_INANITY_GRAPHICS

/// Класс области вывода в X11.
/** Пока всегда целое окно. */
class X11Output : public Output
{
private:
	ptr<X11Window> window;

public:
	X11Output(ptr<X11Window> window);

	ptr<X11Window> GetWindow() const;
};

END_INANITY_GRAPHICS

#endif
