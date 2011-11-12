#ifndef ___INANITY_INANITY_GRAPHICS_HPP___
#define ___INANITY_INANITY_GRAPHICS_HPP___

/* Файл, содержащий ссылки на конкретную реализацию графической подсистемы.
 */

// В Windows используется подсистема DX, использующая DirectX 11
#ifdef ___INANITY_WINDOWS
#define ___INANITY_GRAPHICS_DX
#else
#error There is no implemented graphics system for current platform.
#endif

#include "graphics/graphics.hpp"
#include "graphics/System.hpp"
#include "graphics/Texture.hpp"
#include "graphics/Window.hpp"
#include "graphics/PixelFormat.hpp"
#include "graphics/Geometry.hpp"
#include "graphics/GeometryFormat.hpp"
#include "graphics/GeometrySemantic.hpp"

#endif
