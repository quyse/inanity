#include "GlUniformBuffer.hpp"

GlUniformBuffer::GlUniformBuffer(GLuint name, size_t size)
: UniformBuffer(size), name(name) {}

GlUniformBuffer::~GlUniformBuffer()
{
	glDeleteBuffers(1, &name);
}

GLuint GlUniformBuffer::GetName() const
{
	return name;
}
