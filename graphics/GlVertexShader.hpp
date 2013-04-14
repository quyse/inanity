#ifndef ___INANITY_GRAPHICS_GL_VERTEX_SHADER_HPP___
#define ___INANITY_GRAPHICS_GL_VERTEX_SHADER_HPP___

#include "VertexShader.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

class GlShaderBindings;

/// Класс вершинного шейдера OpenGL.
class GlVertexShader : public VertexShader
{
private:
	GLuint shaderName;
	ptr<GlShaderBindings> shaderBindings;

public:
	GlVertexShader(GLuint shaderName, ptr<GlShaderBindings> shaderBindings);
	~GlVertexShader();

	GLuint GetShaderName() const;
	ptr<GlShaderBindings> GetShaderBindings() const;
};

END_INANITY_GRAPHICS

#endif
