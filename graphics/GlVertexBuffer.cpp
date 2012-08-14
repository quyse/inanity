#include "GlVertexBuffer.hpp"
#include "VertexLayout.hpp"

GlVertexBuffer::GlVertexBuffer(GLuint name, int verticesCount, ptr<VertexLayout> vertexLayout)
: VertexBuffer(verticesCount, vertexLayout), name(name) {}

GlVertexBuffer::~GlVertexBuffer()
{
	glDeleteBuffers(1, &name);
}

GLuint GlVertexBuffer::GetName() const
{
	return name;
}
