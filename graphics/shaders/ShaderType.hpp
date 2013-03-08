#ifndef ___INANITY_GRAPHICS_SHADERS_SHADER_TYPE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_SHADER_TYPE_HPP___

#include "shaders.hpp"

BEGIN_INANITY_SHADERS

/// Типы шейдеров.
struct ShaderTypes
{
	enum _
	{
		vertex,
		pixel
	};
};
typedef ShaderTypes::_ ShaderType;

END_INANITY_SHADERS

#endif
