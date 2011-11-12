#include "DepthStencilBuffer.hpp"
#include "Texture.hpp"

DepthStencilBuffer::DepthStencilBuffer(ID3D11DepthStencilView* depthStencilView, ptr<Texture> texture) :
	depthStencilView(depthStencilView), texture(texture)
{
}

ID3D11DepthStencilView* DepthStencilBuffer::GetDepthStencilView() const
{
	return depthStencilView;
}

ptr<Texture> DepthStencilBuffer::GetAsTexture()
{
	return texture;
}
