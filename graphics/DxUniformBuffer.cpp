#include "DxUniformBuffer.hpp"

DxUniformBuffer::DxUniformBuffer(ID3D11Buffer* buffer, size_t size)
: UniformBuffer(size), buffer(buffer)
{
}

ID3D11Buffer* DxUniformBuffer::GetBufferInterface() const
{
	return buffer;
}
