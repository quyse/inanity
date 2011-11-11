#include "Texture.hpp"

Texture::Texture(ID3D11ShaderResourceView* shaderResourceView) : shaderResourceView(shaderResourceView)
{
}

ID3D11ShaderResourceView* Texture::GetShaderResource() const
{
	return shaderResourceView;
}
