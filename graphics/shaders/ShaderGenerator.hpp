#ifndef ___INANITY_GRAPHICS_SHADERS_SHADER_GENERATOR_HPP___
#define ___INANITY_GRAPHICS_SHADERS_SHADER_GENERATOR_HPP___

#include "ShaderType.hpp"
#include "../../String.hpp"

BEGIN_INANITY_SHADERS

class Expression;

/// Абстрактный класс генератора шейдеров.
class ShaderGenerator : public Object
{
public:
	virtual String Generate(Expression code, ShaderType shaderType) = 0;
};

END_INANITY_SHADERS

#endif
