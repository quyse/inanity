#ifndef ___INANITY_GRAPHICS_SHADERS_HPP___
#define ___INANITY_GRAPHICS_SHADERS_HPP___

/*
 * Общий файл для подсистемы шейдеров.
 * Подсистема шейдеров предназначена для отделения графической системы от
 * конкретных языков шейдеров, таких как HLSL или GLSL.
 */

#include "../graphics.hpp"

#define BEGIN_INANITY_SHADERS BEGIN_INANITY namespace Shaders {
#define END_INANITY_SHADERS } END_INANITY

// Выбор языка для генерации шейдеров.
#ifdef ___INANITY_GRAPHICS_DX
#define ___INANITY_SHADERS_HLSL
#endif
#ifdef ___INANITY_GRAPHICS_GL
#define ___INANITY_SHADERS_GLSL
#endif


BEGIN_INANITY_SHADERS
using namespace Inanity::Math;
END_INANITY_SHADERS
#ifdef INANITY_LIB
using namespace Shaders;
#endif

#endif
