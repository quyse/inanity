#ifndef ___INANITY_GRAPHICS_SHADERS_HLSL_GENERATOR_HPP___
#define ___INANITY_GRAPHICS_SHADERS_HLSL_GENERATOR_HPP___

#include "ShaderGenerator.hpp"

BEGIN_INANITY_SHADERS

/// Генератор HLSL-кода по предоставленному определению шейдера.
class HlslGenerator : public ShaderGenerator
{
public:
	ptr<ShaderSource> Generate(Expression code, ShaderType shaderType);
};

END_INANITY_SHADERS

#endif
