#ifndef ___INANITY_GRAPHICS_GL_VERTEX_SHADER_HPP___
#define ___INANITY_GRAPHICS_GL_VERTEX_SHADER_HPP___

#include "VertexShader.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

class GlDevice;

/// Класс вершинного шейдера OpenGL.
class GlVertexShader : public VertexShader
{
private:
	ptr<GlDevice> device;
	GLuint programName;
	GLuint shaderName;

public:
	GlVertexShader(ptr<GlDevice> device, GLuint programName, GLuint shaderName);
};

END_INANITY_GRAPHICS

#endif
