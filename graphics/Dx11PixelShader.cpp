#include "Dx11PixelShader.hpp"

BEGIN_INANITY_GRAPHICS

Dx11PixelShader::Dx11PixelShader(ComPointer<ID3D11PixelShader> pixelShader, const Dx11ShaderResources& resources)
: pixelShader(pixelShader), resources(resources)
{
}

ID3D11PixelShader* Dx11PixelShader::GetPixelShaderInterface() const
{
	return pixelShader;
}

const Dx11ShaderResources& Dx11PixelShader::GetResources() const
{
	return resources;
}

END_INANITY_GRAPHICS
