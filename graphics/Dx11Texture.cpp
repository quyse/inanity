#include "Dx11Texture.hpp"

BEGIN_INANITY_GRAPHICS

Dx11Texture::Dx11Texture(ComPointer<ID3D11ShaderResourceView> shaderResourceView)
: shaderResourceView(shaderResourceView)
{
}

ID3D11ShaderResourceView* Dx11Texture::GetShaderResourceViewInterface()
{
	return shaderResourceView;
}

END_INANITY_GRAPHICS
