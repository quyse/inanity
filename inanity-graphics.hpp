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
#include "graphics/RenderBuffer.hpp"
#include "graphics/DepthStencilBuffer.hpp"
#include "graphics/RenderStage.hpp"
#include "graphics/Context.hpp"
#include "graphics/Window.hpp"
#include "graphics/Geometry.hpp"
#include "graphics/GeometryFormat.hpp"
#include "graphics/GeometrySemantic.hpp"
#include "graphics/EditableGeometry.hpp"
#include "graphics/FontChar.hpp"
#include "graphics/EditableFont.hpp"
#include "graphics/Font.hpp"
#include "graphics/FontDrawer.hpp"

#endif
