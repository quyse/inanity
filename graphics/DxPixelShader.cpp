#include "DxPixelShader.hpp"

BEGIN_INANITY_GRAPHICS

DxPixelShader::DxPixelShader(ComPointer<ID3D11PixelShader> pixelShader)
: pixelShader(pixelShader)
{
}

ID3D11PixelShader* DxPixelShader::GetPixelShaderInterface() const
{
	return pixelShader;
}

END_INANITY_GRAPHICS
