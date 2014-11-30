#include "UniformBuffer.hpp"

BEGIN_INANITY_GRAPHICS

UniformBuffer::UniformBuffer(int size) : size(size) {}

int UniformBuffer::GetSize() const
{
	return size;
}

END_INANITY_GRAPHICS
