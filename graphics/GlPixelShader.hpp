#ifndef ___INANITY_GRAPHICS_GL_PIXEL_SHADER_HPP___
#define ___INANITY_GRAPHICS_GL_PIXEL_SHADER_HPP___

#include "PixelShader.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс пиксельного (фрагментного) шейдера OpenGL.
class GlPixelShader : public PixelShader
{
private:
	ptr<GlDevice> device;
	GLuint programName;
	GLuint shaderName;

public:
	GlPixelShader(ptr<GlDevice> device, GLuint programName, GLuint shaderName);
};

END_INANITY_GRAPHICS

#endif
