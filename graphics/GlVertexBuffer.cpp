#include "GlVertexBuffer.hpp"
#include "Layout.hpp"

GlVertexBuffer::GlVertexBuffer(GLuint name, int verticesCount, ptr<Layout> layout)
: VertexBuffer(verticesCount, layout), name(name) {}

GlVertexBuffer::~GlVertexBuffer()
{
	glDeleteBuffers(1, &name);
}

GLuint GlVertexBuffer::GetName() const
{
	return name;
}
