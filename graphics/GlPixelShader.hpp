#ifndef ___INANITY_GRAPHICS_GL_PIXEL_SHADER_HPP___
#define ___INANITY_GRAPHICS_GL_PIXEL_SHADER_HPP___

#include "PixelShader.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс пиксельного (фрагментного) шейдера OpenGL.
class GlPixelShader : public PixelShader
{
private:
	GLuint shaderName;

public:
	GlPixelShader(GLuint shaderName);
	~GlPixelShader();

	GLuint GetShaderName() const;
};

END_INANITY_GRAPHICS

#endif
