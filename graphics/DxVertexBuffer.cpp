#include "DxVertexBuffer.hpp"
#include "VertexLayout.hpp"

DxVertexBuffer::DxVertexBuffer(ID3D11Buffer* buffer, int verticesCount, ptr<VertexLayout> vertexLayout)
: VertexBuffer(verticesCount, vertexLayout), buffer(buffer) {}

ID3D11Buffer* DxVertexBuffer::GetBufferInterface() const
{
	return buffer;
}
