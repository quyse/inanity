#ifndef ___INANITY_GRAPHICS_HLSL_GENERATOR_HPP___
#define ___INANITY_GRAPHICS_HLSL_GENERATOR_HPP___

#include "ShaderGenerator.hpp"

BEGIN_INANITY_GRAPHICS

class HlslGenerator : public ShaderGenerator
{
public:
	String Generate(const Shaders::Shader& shader);
};

END_INANITY_GRAPHICS

#endif
