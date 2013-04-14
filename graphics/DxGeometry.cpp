#include "DxGeometry.hpp"
#include "DxVertexBuffer.hpp"
#include "DxIndexBuffer.hpp"

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
