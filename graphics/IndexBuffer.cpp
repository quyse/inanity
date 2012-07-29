#include "IndexBuffer.hpp"

IndexBuffer::IndexBuffer(size_t indicesCount) : indicesCount(indicesCount) {}

size_t IndexBuffer::GetIndicesCount() const
{
	return indicesCount;
}
