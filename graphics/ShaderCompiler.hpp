#ifndef ___INANITY_GRAPHICS_SHADER_COMPILER_HPP___
#define ___INANITY_GRAPHICS_SHADER_COMPILER_HPP___

#include "graphics.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_GRAPHICS

class ShaderSource;

/// Абстрактный класс компилятора шейдеров.
class ShaderCompiler : public Object
{
public:
	/// Скомпилировать шейдер из исходного кода в файл.
	virtual ptr<File> Compile(ptr<ShaderSource> shaderSource) = 0;
};

END_INANITY_GRAPHICS

#endif
