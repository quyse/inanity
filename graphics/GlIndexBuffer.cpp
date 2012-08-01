#include "GlIndexBuffer.hpp"

GlIndexBuffer::GlIndexBuffer(GLuint name, size_t indicesCount)
: IndexBuffer(indicesCount), name(name) {}

GlIndexBuffer::~GlIndexBuffer()
{
	glDeleteBuffers(1, &name);
}

GLuint GlIndexBuffer::GetName() const
{
	return name;
}
