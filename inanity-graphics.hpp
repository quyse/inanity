#ifndef ___INANITY_INANITY_GRAPHICS_HPP___
#define ___INANITY_INANITY_GRAPHICS_HPP___

/* Файл, содержащий ссылки на конкретную реализацию графической подсистемы.
 */

#ifdef ___INANITY_WINDOWS___

// В Windows используется подсистема DX, использующая DirectX 11

// Заголовочные файлы.

#include "dx/System.hpp"

// Определения.

namespace Graphics
{
	typedef DX::System System;
	typedef DX::Texture Texture;
	typedef DX::ConstantBuffer ConstantBuffer;
	typedef DX::Geometry Geometry;
	typedef DX::GeometryFormat GeometryFormat;
};

#endif

#endif
