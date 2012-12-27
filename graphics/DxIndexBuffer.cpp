#include "DxIndexBuffer.hpp"

DxIndexBuffer::DxIndexBuffer(ComPointer<ID3D11Buffer> buffer, int indicesCount, int indexSize)
: IndexBuffer(indicesCount, indexSize), buffer(buffer) {}

ID3D11Buffer* DxIndexBuffer::GetBufferInterface() const
{
	return buffer;
}
