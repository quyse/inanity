#include "DxIndexBuffer.hpp"

BEGIN_INANITY_GRAPHICS

DxIndexBuffer::DxIndexBuffer(ComPointer<ID3D11Buffer> buffer, int indicesCount, int indexSize)
: IndexBuffer(indicesCount, indexSize), buffer(buffer) {}

ID3D11Buffer* DxIndexBuffer::GetBufferInterface() const
{
	return buffer;
}

DXGI_FORMAT DxIndexBuffer::GetIndexFormat() const
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
