#include "VertexBuffer.hpp"
#include "VertexLayout.hpp"

VertexBuffer::VertexBuffer(int verticesCount, ptr<VertexLayout> vertexLayout)
: verticesCount(verticesCount), vertexLayout(vertexLayout) {}

int VertexBuffer::GetVerticesCount() const
{
	return verticesCount;
}

ptr<VertexLayout> VertexBuffer::GetVertexLayout() const
{
	return vertexLayout;
}
