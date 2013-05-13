#include "Dx11UniformBuffer.hpp"

BEGIN_INANITY_GRAPHICS

Dx11UniformBuffer::Dx11UniformBuffer(ComPointer<ID3D11Buffer> buffer, size_t size)
: UniformBuffer(size), buffer(buffer)
{
}

ID3D11Buffer* Dx11UniformBuffer::GetBufferInterface() const
{
	return buffer;
}

END_INANITY_GRAPHICS
