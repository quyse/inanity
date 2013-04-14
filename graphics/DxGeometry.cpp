#include "DxGeometry.hpp"
#include "DxVertexBuffer.hpp"
#include "DxIndexBuffer.hpp"

BEGIN_INANITY_GRAPHICS

DxGeometry::DxGeometry(ptr<DxVertexBuffer> vertexBuffer, ptr<DxIndexBuffer> indexBuffer)
: vertexBuffer(vertexBuffer), indexBuffer(indexBuffer) {}

ptr<DxVertexBuffer> DxGeometry::GetVertexBuffer() const
{
	return vertexBuffer;
}

ptr<DxIndexBuffer> DxGeometry::GetIndexBuffer() const
{
	return indexBuffer;
}

END_INANITY_GRAPHICS
