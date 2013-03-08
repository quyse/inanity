#include "DxTexture.hpp"

DxTexture::DxTexture(ComPointer<ID3D11ShaderResourceView> shaderResourceView)
: shaderResourceView(shaderResourceView)
{
}

ID3D11ShaderResourceView* DxTexture::GetShaderResourceViewInterface()
{
	return shaderResourceView;
}
