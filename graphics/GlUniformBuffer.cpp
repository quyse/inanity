#include "GlUniformBuffer.hpp"
#include "GlDevice.hpp"
#include "../MemoryFile.hpp"

BEGIN_INANITY_GRAPHICS

GlUniformBuffer::GlUniformBuffer(ptr<GlDevice> device, GLuint name, size_t size)
: UniformBuffer(size), device(device), name(name)
{
	if(!name)
		file = NEW(MemoryFile(size));
}

GlUniformBuffer::~GlUniformBuffer()
{
	if(name)
		glDeleteBuffers(1, &name);
}

GLuint GlUniformBuffer::GetName() const
{
	return name;
}

END_INANITY_GRAPHICS
