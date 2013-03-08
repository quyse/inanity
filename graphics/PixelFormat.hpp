#ifndef ___INANITY_GRAPHICS_PIXEL_FORMAT_HPP___
#define ___INANITY_GRAPHICS_PIXEL_FORMAT_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

/// Форматы пикселов.
struct PixelFormats
{
	enum _
	{
		unknown,
		intR8G8B8A8,
		floatR11G11B10,
		floatR16,
		typelessR32,
		floatR32,
		floatR32Depth,
	};
};
typedef PixelFormats::_ PixelFormat;

END_INANITY_GRAPHICS

#endif
