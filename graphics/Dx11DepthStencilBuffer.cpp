#include "Dx11DepthStencilBuffer.hpp"
#include "Dx11Texture.hpp"

BEGIN_INANITY_GRAPHICS

Dx11DepthStencilBuffer::Dx11DepthStencilBuffer(ComPointer<ID3D11DepthStencilView> depthStencilView, ptr<Dx11Texture> texture)
: depthStencilView(depthStencilView), texture(texture)
{
}

ptr<Texture> Dx11DepthStencilBuffer::GetTexture()
{
	return texture;
}

ID3D11DepthStencilView* Dx11DepthStencilBuffer::GetDepthStencilViewInterface()
{
	return depthStencilView;
}

END_INANITY_GRAPHICS
