#include "GlVertexBuffer.hpp"

GlVertexBuffer::GlVertexBuffer(GLuint name, size_t verticesCount)
: VertexBuffer(verticesCount), name(name) {}

GlVertexBuffer::~GlVertexBuffer()
{
	glDeleteBuffers(1, &name);
}

GLuint GlVertexBuffer::GetName() const
{
	return name;
}
