#include "VertexBuffer.hpp"
#include "Layout.hpp"

BEGIN_INANITY_GRAPHICS

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

END_INANITY_GRAPHICS
