#ifndef ___INANITY_GRAPHICS_SHADER_GENERATOR_HPP___
#define ___INANITY_GRAPHICS_SHADER_GENERATOR_HPP___

#include "graphics.hpp"
#include "../String.hpp"

BEGIN_INANITY_GRAPHICS

/// Абстрактный класс генератора шейдеров.
class ShaderGenerator : public Object
{
public:
	virtual String Generate(const Shaders::Shader& shader) = 0;
};

END_INANITY_GRAPHICS

#endif
