#include "UniformBuffer.hpp"

BEGIN_INANITY_GRAPHICS

UniformBuffer::UniformBuffer(size_t size) : size(size) {}

size_t UniformBuffer::GetSize() const
{
	return size;
}

END_INANITY_GRAPHICS
