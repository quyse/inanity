#include "RenderBuffer.hpp"
#include "Texture.hpp"
#include "../../Exception.hpp"

RenderBuffer::RenderBuffer(ID3D11RenderTargetView* renderTargetView, ptr<Texture> texture) :
	renderTargetView(renderTargetView), texture(texture)
{
	// получить размеры буфера
	try
	{
		ID3D11Resource* resource;
		renderTargetView->GetResource(&resource);

		ID3D11Texture2D* texture;
		HRESULT hr = resource->QueryInterface(IID_ID3D11Texture2D, (void**)&texture);
		resource->Release();
		if(FAILED(hr))
			THROW_PRIMARY_EXCEPTION("Can't get texture 2D interface of resource");

		D3D11_TEXTURE2D_DESC desc;
		texture->GetDesc(&desc);
		texture->Release();

		width = desc.Width;
		height = desc.Height;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't get size of render buffer", exception);
	}
}

size_t RenderBuffer::GetWidth() const
{
	return width;
}

size_t RenderBuffer::GetHeight() const
{
	return height;
}

ID3D11RenderTargetView* RenderBuffer::GetRenderTargetView() const
{
	return renderTargetView;
}

ptr<Texture> RenderBuffer::GetAsTexture() const
{
	return texture;
}
