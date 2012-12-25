#include "DxIndexBuffer.hpp"

DxIndexBuffer::DxIndexBuffer(ComPointer<ID3D11Buffer> buffer, size_t indicesCount)
: IndexBuffer(indicesCount), buffer(buffer) {}

ID3D11Buffer* DxIndexBuffer::GetBufferInterface() const
{
	return buffer;
}
