#include "Dx11VertexBuffer.hpp"
#include "VertexLayout.hpp"

BEGIN_INANITY_GRAPHICS

Dx11VertexBuffer::Dx11VertexBuffer(ComPointer<ID3D11Buffer> buffer, int verticesCount, ptr<VertexLayout> layout)
: VertexBuffer(verticesCount, layout), buffer(buffer) {}

ID3D11Buffer* Dx11VertexBuffer::GetBufferInterface() const
{
	return buffer;
}

END_INANITY_GRAPHICS
