#include "DxDepthStencilBuffer.hpp"
#include "DxTexture.hpp"

BEGIN_INANITY_GRAPHICS

DxDepthStencilBuffer::DxDepthStencilBuffer(ComPointer<ID3D11DepthStencilView> depthStencilView, ptr<DxTexture> texture)
: depthStencilView(depthStencilView), texture(texture)
{
}

ptr<Texture> DxDepthStencilBuffer::GetTexture()
{
	return texture;
}

ID3D11DepthStencilView* DxDepthStencilBuffer::GetDepthStencilViewInterface()
{
	return depthStencilView;
}

END_INANITY_GRAPHICS
