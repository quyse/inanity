#include "DxTexture.hpp"

BEGIN_INANITY_GRAPHICS

DxTexture::DxTexture(ComPointer<ID3D11ShaderResourceView> shaderResourceView)
: shaderResourceView(shaderResourceView)
{
}

ID3D11ShaderResourceView* DxTexture::GetShaderResourceViewInterface()
{
	return shaderResourceView;
}

END_INANITY_GRAPHICS
