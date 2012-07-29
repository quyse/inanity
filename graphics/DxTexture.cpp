#include "DxTexture.hpp"

DxTexture::DxTexture(ID3D11ShaderResourceView* shaderResourceView)
: shaderResourceView(shaderResourceView)
{
}

ID3D11ShaderResourceView* DxTexture::GetShaderResourceViewInterface()
{
	return shaderResourceView;
}
