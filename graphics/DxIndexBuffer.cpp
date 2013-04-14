#include "DxIndexBuffer.hpp"

BEGIN_INANITY_GRAPHICS

DxIndexBuffer::DxIndexBuffer(ComPointer<ID3D11Buffer> buffer, int indicesCount, int indexSize)
: IndexBuffer(indicesCount, indexSize), buffer(buffer) {}

ID3D11Buffer* DxIndexBuffer::GetBufferInterface() const
{
	return buffer;
}

END_INANITY_GRAPHICS
