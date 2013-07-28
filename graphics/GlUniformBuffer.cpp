#include "GlUniformBuffer.hpp"
#include "GlDevice.hpp"

BEGIN_INANITY_GRAPHICS

GlUniformBuffer::GlUniformBuffer(ptr<GlDevice> device, GLuint name, size_t size)
: UniformBuffer(size), device(device), name(name) {}

GlUniformBuffer::~GlUniformBuffer()
{
	glDeleteBuffers(1, &name);
}

GLuint GlUniformBuffer::GetName() const
{
	return name;
}

END_INANITY_GRAPHICS
