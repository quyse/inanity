#ifndef ___INANITY_DX_PIXEL_FORMAT_HPP___
#define ___INANITY_DX_PIXEL_FORMAT_HPP___

#include "dx.hpp"
#include "d3d.hpp"

BEGIN_INANITY_DX

enum PixelFormat
{
	pixelFormatIntR8G8B8A8 = DXGI_FORMAT_R8G8B8A8_UNORM,
	pixelFormatFloatR11G11B10 = DXGI_FORMAT_R11G11B10_FLOAT,
	pixelFormatIntRGBA32 = pixelFormatIntR8G8B8A8,
	pixelFormatFloatRGB32 = pixelFormatFloatR11G11B10
};

END_INANITY_DX

#endif
