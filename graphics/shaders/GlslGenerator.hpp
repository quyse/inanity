#ifndef ___INANITY_GRAPHICS_SHADERS_GLSL_GENERATOR_HPP___
#define ___INANITY_GRAPHICS_SHADERS_GLSL_GENERATOR_HPP___

#include "ShaderGenerator.hpp"
#include "GlslVersion.hpp"

BEGIN_INANITY_SHADERS

/// GLSL code generator.
class GlslGenerator : public ShaderGenerator
{
private:
	GlslVersion glslVersion;
	bool supportUniformBuffers;

public:
	GlslGenerator(GlslVersion glslVersion, bool supportUniformBuffers);

	ptr<ShaderSource> Generate(Expression code, ShaderType shaderType);
};

END_INANITY_SHADERS

#endif
