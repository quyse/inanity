#ifndef ___INANITY_GRAPHICS_SHADERS_SHADER_GENERATOR_HPP___
#define ___INANITY_GRAPHICS_SHADERS_SHADER_GENERATOR_HPP___

#include "ShaderType.hpp"

BEGIN_INANITY_GRAPHICS

class ShaderSource;

END_INANITY_GRAPHICS

BEGIN_INANITY_SHADERS

class Expression;

/// Абстрактный класс генератора шейдеров.
class ShaderGenerator : public Object
{
public:
	virtual ptr<ShaderSource> Generate(Expression code, ShaderType shaderType) = 0;
};

END_INANITY_SHADERS

#endif
