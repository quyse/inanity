#include "GlIndexBuffer.hpp"
#include "GlDevice.hpp"

BEGIN_INANITY_GRAPHICS

GlIndexBuffer::GlIndexBuffer(ptr<GlDevice> device, GLuint name, int indicesCount, int indexSize)
: IndexBuffer(indicesCount, indexSize), device(device), name(name) {}

GlIndexBuffer::~GlIndexBuffer()
{
	glDeleteBuffers(1, &name);
}

GLuint GlIndexBuffer::GetName() const
{
	return name;
}

GLenum GlIndexBuffer::GetIndicesType() const
{
	switch(indexSize)
	{
	case 1:
		return GL_UNSIGNED_BYTE;
	case 2:
		return GL_UNSIGNED_SHORT;
	case 4:
		return GL_UNSIGNED_INT;
	default:
		return 0;
	}
}

END_INANITY_GRAPHICS
