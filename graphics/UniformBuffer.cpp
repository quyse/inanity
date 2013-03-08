#include "UniformBuffer.hpp"

UniformBuffer::UniformBuffer(size_t size) : size(size) {}

size_t UniformBuffer::GetSize() const
{
	return size;
}
