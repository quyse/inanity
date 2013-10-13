#include "EmsOutput.hpp"
#include <emscripten/emscripten.h>

BEGIN_INANITY_GRAPHICS

EmsOutput::EmsOutput() {}

int EmsOutput::GetWidth() const
{
	int width;
	emscripten_get_canvas_size(&width, nullptr, nullptr);
	return width;
}

int EmsOutput::GetHeight() const
{
	int height;
	emscripten_get_canvas_size(nullptr, &height, nullptr);
	return height;
}

END_INANITY_GRAPHICS
