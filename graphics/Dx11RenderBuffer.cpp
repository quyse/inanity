#include "Dx11RenderBuffer.hpp"
#include "Dx11Texture.hpp"

BEGIN_INANITY_GRAPHICS

Dx11RenderBuffer::Dx11RenderBuffer(ComPointer<ID3D11RenderTargetView> renderTargetView, ptr<Dx11Texture> texture)
: renderTargetView(renderTargetView), texture(texture)
{
}

ptr<Texture> Dx11RenderBuffer::GetTexture()
{
	return texture;
}

ID3D11RenderTargetView* Dx11RenderBuffer::GetRenderTargetViewInterface() const
{
	return renderTargetView;
}

float Dx11RenderBuffer::GetScreenTopTexcoord() const
{
	return 0;
}

END_INANITY_GRAPHICS
