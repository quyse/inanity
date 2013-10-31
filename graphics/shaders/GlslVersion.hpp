#ifndef ___INANITY_GRAPHICS_SHADERS_GLSL_VERSION_HPP___
#define ___INANITY_GRAPHICS_SHADERS_GLSL_VERSION_HPP___

#include "shaders.hpp"

BEGIN_INANITY_SHADERS

/// GLSL versions.
struct GlslVersions
{
	enum _
	{
		opengl33,
		webgl
	};
};
typedef GlslVersions::_ GlslVersion;

END_INANITY_SHADERS

#endif
