#ifndef ___INANITY_GRAPHICS_SHADERS_HLSL11_GENERATOR_HPP___
#define ___INANITY_GRAPHICS_SHADERS_HLSL11_GENERATOR_HPP___

#include "ShaderGenerator.hpp"

BEGIN_INANITY_SHADERS

/// Генератор HLSL-кода по предоставленному определению шейдера.
class Hlsl11Generator : public ShaderGenerator
{
public:
	ptr<ShaderSource> Generate(Expression code, ShaderType shaderType);
};

END_INANITY_SHADERS

#endif
