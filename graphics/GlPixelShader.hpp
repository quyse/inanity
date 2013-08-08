#ifndef ___INANITY_GRAPHICS_GL_PIXEL_SHADER_HPP___
#define ___INANITY_GRAPHICS_GL_PIXEL_SHADER_HPP___

#include "PixelShader.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

class GlDevice;
class GlShaderBindings;

/// Класс пиксельного (фрагментного) шейдера OpenGL.
class GlPixelShader : public PixelShader
{
private:
	ptr<GlDevice> device;
	GLuint shaderName;
	ptr<GlShaderBindings> shaderBindings;

public:
	GlPixelShader(ptr<GlDevice> device, GLuint shaderName, ptr<GlShaderBindings> shaderBindings);
	~GlPixelShader();

	GLuint GetShaderName() const;
	ptr<GlShaderBindings> GetShaderBindings() const;
};

END_INANITY_GRAPHICS

#endif
