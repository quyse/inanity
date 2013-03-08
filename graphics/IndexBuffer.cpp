#include "IndexBuffer.hpp"

IndexBuffer::IndexBuffer(int indicesCount, int indexSize)
: indicesCount(indicesCount), indexSize(indexSize) {}

int IndexBuffer::GetIndicesCount() const
{
	return indicesCount;
}

int IndexBuffer::GetIndexSize() const
{
	return indexSize;
}
