#include "DxUniformBuffer.hpp"

DxUniformBuffer::DxUniformBuffer(ID3D11Buffer* buffer, size_t size)
: buffer(buffer), size(size)
{
}

size_t DxUniformBuffer::GetSize()
{
	return size;
}

ID3D11Buffer* DxUniformBuffer::GetBufferInterface() const
{
	return buffer;
}
