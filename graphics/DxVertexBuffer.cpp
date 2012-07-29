#include "DxVertexBuffer.hpp"

DxVertexBuffer::DxVertexBuffer(ID3D11Buffer* buffer) : buffer(buffer) {}

ID3D11Buffer* DxVertexBuffer::GetBufferInterface() const
{
	return buffer;
}
