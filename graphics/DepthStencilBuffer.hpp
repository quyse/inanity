#ifndef ___INANITY_DEPTH_STENCIL_BUFFER_HPP___
#define ___INANITY_DEPTH_STENCIL_BUFFER_HPP___

#include "graphics.hpp"

#ifdef ___INANITY_GRAPHICS_DX

#include "dx/DepthStencilBuffer.hpp"
BEGIN_INANITY_GRAPHICS
typedef DX::DepthStencilBuffer DepthStencilBuffer;
END_INANITY_GRAPHICS

#endif

#endif
