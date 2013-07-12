#ifndef ___INANITY_GRAPHICS_X11_OUTPUT_HPP___
#define ___INANITY_GRAPHICS_X11_OUTPUT_HPP___

#include "Output.hpp"
#include "../platform/platform.hpp"

BEGIN_INANITY_PLATFORM

class X11Window;

END_INANITY_PLATFORM

BEGIN_INANITY_GRAPHICS

/// X11 graphics output class.
class X11Output : public Output
{
private:
	ptr<Platform::X11Window> window;
	int width, height;

public:
	X11Output(ptr<Platform::X11Window> window);

	// Output's methods.
	int GetWidth() const;
	int GetHeight() const;

	ptr<Platform::X11Window> GetWindow() const;
	void Resize(int width, int height);
};

END_INANITY_GRAPHICS

#endif
