#include "DxIndexBuffer.hpp"

DxIndexBuffer::DxIndexBuffer(ID3D11Buffer* buffer, size_t size)
: IndexBuffer(size), buffer(buffer) {}

ID3D11Buffer* DxIndexBuffer::GetBufferInterface() const
{
	return buffer;
}
