#ifndef ___INANITY_GRAPHICS_SHADERS_HLSL_GENERATOR_HPP___
#define ___INANITY_GRAPHICS_SHADERS_HLSL_GENERATOR_HPP___

#include "ShaderGenerator.hpp"

BEGIN_INANITY_SHADERS

/// Генератор HLSL-кода по предоставленному определению шейдера.
class HlslGenerator : public ShaderGenerator
{
public:
	String Generate(const Shader& shader);
};

END_INANITY_SHADERS

#endif
