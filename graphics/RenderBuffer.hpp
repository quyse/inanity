#ifndef ___INANITY_GRAPHICS_RENDER_BUFFER_HPP___
#define ___INANITY_GRAPHICS_RENDER_BUFFER_HPP___

#include "graphics.hpp"

#ifdef ___INANITY_GRAPHICS_DX

#include "dx/RenderBuffer.hpp"
BEGIN_INANITY_GRAPHICS
typedef DX::RenderBuffer RenderBuffer;
END_INANITY_GRAPHICS

#endif

#endif
