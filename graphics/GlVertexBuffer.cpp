#include "GlVertexBuffer.hpp"
#include "Layout.hpp"

BEGIN_INANITY_GRAPHICS

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

END_INANITY_GRAPHICS
