#include "DxVertexBuffer.hpp"

DxVertexBuffer::DxVertexBuffer(ID3D11Buffer* buffer, size_t size)
: VertexBuffer(size), buffer(buffer) {}

ID3D11Buffer* DxVertexBuffer::GetBufferInterface() const
{
	return buffer;
}
