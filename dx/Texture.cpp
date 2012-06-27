#include "Texture.hpp"
#include "System.hpp"
#include "../../File.hpp"
#include "../../ResourceLoader.hpp"
#include "../../ResourceManager.hpp"
#include "../../Exception.hpp"
#include "d3dx.hpp"

Texture::Texture(ID3D11ShaderResourceView* shaderResourceView) : shaderResourceView(shaderResourceView)
{
}

ID3D11ShaderResourceView* Texture::GetShaderResource() const
{
	return shaderResourceView;
}

ptr<Texture> Texture::Deserialize(ptr<File> file, ResourceLoader* resourceLoader)
{
	try
	{
		D3DX11_IMAGE_INFO info;
		if(FAILED(D3DX11GetImageInfoFromMemory(file->GetData(), file->GetSize(), 0, &info, NULL)))
			THROW_PRIMARY_EXCEPTION("Can't get image info");

		D3DX11_IMAGE_LOAD_INFO loadInfo;
		loadInfo.Width = info.Width;
		loadInfo.Height = info.Height;
		loadInfo.Depth = info.Depth;
		loadInfo.FirstMipLevel = 0;
		loadInfo.MipLevels = info.MipLevels;
		loadInfo.Usage = D3D11_USAGE_IMMUTABLE;
		loadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		loadInfo.CpuAccessFlags = 0;
		loadInfo.MiscFlags = info.MiscFlags;
		loadInfo.Format = info.Format;
		loadInfo.Filter = D3DX11_FILTER_LINEAR;
		loadInfo.MipFilter = D3DX11_FILTER_LINEAR;
		loadInfo.pSrcInfo = &info;

		ID3D11ShaderResourceView* shaderResourceView;
		if(FAILED(D3DX11CreateShaderResourceViewFromMemory(resourceLoader->GetStaticResource<System>()->GetDevice(), file->GetData(), file->GetSize(), &loadInfo, 0, &shaderResourceView, NULL)))
			THROW_PRIMARY_EXCEPTION("Can't create shader resource view");

		return NEW(Texture(shaderResourceView));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't load texture", exception);
	}
}
