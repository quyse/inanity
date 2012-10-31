#include "VertexBuffer.hpp"
#include "Layout.hpp"

VertexBuffer::VertexBuffer(int verticesCount, ptr<Layout> layout)
: verticesCount(verticesCount), layout(layout) {}

int VertexBuffer::GetVerticesCount() const
{
	return verticesCount;
}

ptr<Layout> VertexBuffer::GetLayout() const
{
	return layout;
}
