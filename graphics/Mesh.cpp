#include "Mesh.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

BEGIN_INANITY_GRAPHICS

Mesh::Mesh(ptr<VertexBuffer> vertexBuffer, ptr<IndexBuffer> indexBuffer)
: vertexBuffer(vertexBuffer), indexBuffer(indexBuffer) {}

Mesh::~Mesh() {}

ptr<VertexBuffer> Mesh::GetVertexBuffer() const
{
	return vertexBuffer;
}

ptr<IndexBuffer> Mesh::GetIndexBuffer() const
{
	return indexBuffer;
}

END_INANITY_GRAPHICS
