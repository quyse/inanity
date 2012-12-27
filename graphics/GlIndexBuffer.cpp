#include "GlIndexBuffer.hpp"

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
