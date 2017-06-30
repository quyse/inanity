#include "Dx11Texture.hpp"
#include "Dx11SamplerState.hpp"

BEGIN_INANITY_GRAPHICS

Dx11Texture::Dx11Texture(ComPointer<ID3D11ShaderResourceView> shaderResourceView, ptr<Dx11SamplerState> samplerState, int width, int height, int depth)
: Texture(width, height, depth), shaderResourceView(shaderResourceView), samplerState(samplerState)
{}

ID3D11ShaderResourceView* Dx11Texture::GetShaderResourceViewInterface()
{
	return shaderResourceView;
}

ptr<Dx11SamplerState> Dx11Texture::GetSamplerState() const
{
	return samplerState;
}

END_INANITY_GRAPHICS
