#include "DxVertexBuffer.hpp"
#include "VertexLayout.hpp"

BEGIN_INANITY_GRAPHICS

DxVertexBuffer::DxVertexBuffer(ComPointer<ID3D11Buffer> buffer, int verticesCount, ptr<VertexLayout> layout)
: VertexBuffer(verticesCount, layout), buffer(buffer) {}

ID3D11Buffer* DxVertexBuffer::GetBufferInterface() const
{
	return buffer;
}

END_INANITY_GRAPHICS
