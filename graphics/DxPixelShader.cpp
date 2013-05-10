#include "DxPixelShader.hpp"

BEGIN_INANITY_GRAPHICS

DxPixelShader::DxPixelShader(ComPointer<ID3D11PixelShader> pixelShader, const DxShaderResources& resources)
: pixelShader(pixelShader), resources(resources)
{
}

ID3D11PixelShader* DxPixelShader::GetPixelShaderInterface() const
{
	return pixelShader;
}

const DxShaderResources& DxPixelShader::GetResources() const
{
	return resources;
}

END_INANITY_GRAPHICS
