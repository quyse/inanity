#ifndef ___INANITY_GRAPHICS_PIXEL_FORMAT_HPP___
#define ___INANITY_GRAPHICS_PIXEL_FORMAT_HPP___

#include "graphics.hpp"

#ifdef ___INANITY_GRAPHICS_DX

#include "dx/Texture.hpp"
BEGIN_INANITY_GRAPHICS
typedef DX::PixelFormat PixelFormat;
END_INANITY_GRAPHICS

#endif

#endif
