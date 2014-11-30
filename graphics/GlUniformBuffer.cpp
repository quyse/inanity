#include "GlUniformBuffer.hpp"
#include "GlDevice.hpp"
#include "../MemoryFile.hpp"

BEGIN_INANITY_GRAPHICS

GlUniformBuffer::GlUniformBuffer(ptr<GlDevice> device, GLuint name, int size)
: UniformBuffer(size), device(device), name(name), dirty(true)
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

ptr<MemoryFile> GlUniformBuffer::GetFile() const
{
	return file;
}

void GlUniformBuffer::SetDirty(bool dirty)
{
	this->dirty = dirty;
}

bool GlUniformBuffer::IsDirty() const
{
	return dirty;
}

END_INANITY_GRAPHICS
