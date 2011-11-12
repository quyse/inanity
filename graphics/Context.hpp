#ifndef ___INANITY_GRAPHICS_CONTEXT_HPP___
#define ___INANITY_GRAPHICS_CONTEXT_HPP___

#include "graphics.hpp"

#ifdef ___INANITY_GRAPHICS_DX

#include "dx/Context.hpp"
BEGIN_INANITY_GRAPHICS
typedef DX::Context Context;
END_INANITY_GRAPHICS

#endif

#endif
