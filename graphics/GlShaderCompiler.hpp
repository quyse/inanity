#ifndef ___INANITY_GRAPHICS_GL_SHADER_COMPILER_HPP___
#define ___INANITY_GRAPHICS_GL_SHADER_COMPILER_HPP___

#include "ShaderCompiler.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс компилятора шейдеров OpenGL.
class GlShaderCompiler : public ShaderCompiler
{
public:
	ptr<File> Compile(ptr<ShaderSource> shaderSource);
};

END_INANITY_GRAPHICS

#endif
