#include "Dx11IndexBuffer.hpp"

BEGIN_INANITY_GRAPHICS

Dx11IndexBuffer::Dx11IndexBuffer(ComPointer<ID3D11Buffer> buffer, int indicesCount, int indexSize)
: IndexBuffer(indicesCount, indexSize), buffer(buffer) {}

ID3D11Buffer* Dx11IndexBuffer::GetBufferInterface() const
{
	return buffer;
}

DXGI_FORMAT Dx11IndexBuffer::GetIndexFormat() const
{
	switch(indexSize)
	{
	case 2:
		return DXGI_FORMAT_R16_UINT;
	case 4:
		return DXGI_FORMAT_R32_UINT;
	default:
		return DXGI_FORMAT_UNKNOWN;
	}
}

END_INANITY_GRAPHICS
