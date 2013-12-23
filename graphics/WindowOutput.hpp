#ifndef ___INANITY_GRAPHICS_WINDOW_OUTPUT_HPP___
#define ___INANITY_GRAPHICS_WINDOW_OUTPUT_HPP___

#include "Output.hpp"
#include "../platform/platform.hpp"

BEGIN_INANITY_PLATFORM

class Window;

END_INANITY_PLATFORM

BEGIN_INANITY_GRAPHICS

/// Universal output-to-window class.
class WindowOutput : public Output
{
private:
	ptr<Platform::Window> window;
	int width;
	int height;

public:
	WindowOutput(ptr<Platform::Window> window, int width, int height);

	ptr<Platform::Window> GetWindow() const;

	//*** Output's methods.
	int GetWidth() const;
	int GetHeight() const;
	void Resize(int width, int height);
};

END_INANITY_GRAPHICS

#endif
