#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer(size_t verticesCount) : verticesCount(verticesCount) {}

size_t VertexBuffer::GetVerticesCount() const
{
	return verticesCount;
}
