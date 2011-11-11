#ifndef ___INANITY_GRAPHICS_TEXTURE_HPP___
#define ___INANITY_GRAPHICS_TEXTURE_HPP___

#include "graphics.hpp"

#ifdef ___INANITY_GRAPHICS_DX

#include "dx/Texture.hpp"
BEGIN_INANITY_GRAPHICS
typedef DX::Texture Texture;
END_INANITY_GRAPHICS

#endif

#endif
