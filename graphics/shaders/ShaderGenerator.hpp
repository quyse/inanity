#ifndef ___INANITY_GRAPHICS_SHADERS_SHADER_GENERATOR_HPP___
#define ___INANITY_GRAPHICS_SHADERS_SHADER_GENERATOR_HPP___

#include "shaders.hpp"
#include "../../String.hpp"

BEGIN_INANITY_SHADERS

class Shader;

/// Абстрактный класс генератора шейдеров.
class ShaderGenerator : public Object
{
public:
	virtual String Generate(const Shader& shader) = 0;
};

END_INANITY_SHADERS

#endif
