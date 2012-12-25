#include "DxVertexBuffer.hpp"
#include "Layout.hpp"

DxVertexBuffer::DxVertexBuffer(ComPointer<ID3D11Buffer> buffer, int verticesCount, ptr<Layout> layout)
: VertexBuffer(verticesCount, layout), buffer(buffer) {}

ID3D11Buffer* DxVertexBuffer::GetBufferInterface() const
{
	return buffer;
}
