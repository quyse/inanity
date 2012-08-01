#ifndef ___INANITY_GRAPHICS_GL_VERTEX_SHADER_HPP___
#define ___INANITY_GRAPHICS_GL_VERTEX_SHADER_HPP___

#include "VertexShader.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс вершинного шейдера OpenGL.
class GlVertexShader : public VertexShader
{
private:
	GLuint programName;
	GLuint shaderName;

public:
	GlVertexShader(GLuint programName, GLuint shaderName);
	~GlVertexShader();

	GLuint GetProgramName() const;
};

END_INANITY_GRAPHICS

#endif
