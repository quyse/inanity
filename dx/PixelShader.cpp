#include "PixelShader.hpp"
#include "ShaderReflection.hpp"

PixelShader::PixelShader(ptr<ShaderReflection> shaderReflection, ID3D11PixelShader* pixelShader)
: Shader(shaderReflection), pixelShader(pixelShader)
{
}

ID3D11PixelShader* PixelShader::GetInternalInterface() const
{
	return pixelShader;
}
