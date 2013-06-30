#ifndef ___INANITY_GRAPHICS_GL_VERTEX_BUFFER_HPP___
#define ___INANITY_GRAPHICS_GL_VERTEX_BUFFER_HPP___

#include "VertexBuffer.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс вершинного буфера OpenGL.
class GlVertexBuffer : public VertexBuffer
{
private:
	GLuint name;

public:
	GlVertexBuffer(GLuint name, int verticesCount, ptr<VertexLayout> layout);
	~GlVertexBuffer();

	GLuint GetName() const;
};

END_INANITY_GRAPHICS

#endif
