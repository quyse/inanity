#include "IndexBuffer.hpp"

BEGIN_INANITY_GRAPHICS

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

END_INANITY_GRAPHICS
