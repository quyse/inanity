#include "VertexBuffer.hpp"
#include "VertexLayout.hpp"

BEGIN_INANITY_GRAPHICS

VertexBuffer::VertexBuffer(int verticesCount, ptr<VertexLayout> layout)
: verticesCount(verticesCount), layout(layout) {}

int VertexBuffer::GetVerticesCount() const
{
	return verticesCount;
}

ptr<VertexLayout> VertexBuffer::GetLayout() const
{
	return layout;
}

END_INANITY_GRAPHICS
