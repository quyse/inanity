#ifndef ___INANITY_GRAPHICS_GL_SHADER_COMPILER_HPP___
#define ___INANITY_GRAPHICS_GL_SHADER_COMPILER_HPP___

#include "ShaderCompiler.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс компилятора шейдеров OpenGL.
/** Пока является заглушкой, то есть он не транслирует GLSL
в бинарный вид (из-за проблем с бинарными шейдерами).
Он просто возвращает текст шейдера. */
class GlShaderCompiler : public ShaderCompiler
{
public:
	ptr<File> Compile(ptr<ShaderSource> shaderSource);
};

END_INANITY_GRAPHICS

#endif
