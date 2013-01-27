#ifndef ___INANITY_GRAPHICS_SHADERS_GLSL_GENERATOR_HPP___
#define ___INANITY_GRAPHICS_SHADERS_GLSL_GENERATOR_HPP___

#include "ShaderGenerator.hpp"

BEGIN_INANITY_SHADERS

/// Генератор GLSL-кода по предоставленному определению шейдера.
class GlslGenerator : public ShaderGenerator
{
public:
	ptr<ShaderSource> Generate(Expression code, ShaderType shaderType);
};

END_INANITY_SHADERS

#endif
