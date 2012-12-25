#include "DxPixelShader.hpp"

DxPixelShader::DxPixelShader(ComPointer<ID3D11PixelShader> pixelShader)
: pixelShader(pixelShader)
{
}

ID3D11PixelShader* DxPixelShader::GetPixelShaderInterface() const
{
	return pixelShader;
}
