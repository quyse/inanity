#ifndef ___INANITY_GRAPHICS_PRESENT_MODE_HPP___
#define ___INANITY_GRAPHICS_PRESENT_MODE_HPP___

#include "PixelFormat.hpp"

BEGIN_INANITY_GRAPHICS

/// Структура режима экрана.
struct PresentMode
{
	/// Ширина экрана.
	int width;
	/// Высота экрана.
	int height;
	/// Полноэкранный режим?
	bool fullscreen;
	/// Формат пикселов экрана.
	PixelFormat pixelFormat;
};

END_INANITY_GRAPHICS

#endif
