#include "GlIndexBuffer.hpp"

BEGIN_INANITY_GRAPHICS

GlIndexBuffer::GlIndexBuffer(GLuint name, int indicesCount, int indexSize)
: IndexBuffer(indicesCount, indexSize), name(name) {}

GlIndexBuffer::~GlIndexBuffer()
{
	glDeleteBuffers(1, &name);
}

GLuint GlIndexBuffer::GetName() const
{
	return name;
}

END_INANITY_GRAPHICS
