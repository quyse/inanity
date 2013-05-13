#include "Dx11VertexShader.hpp"
#include "../File.hpp"

BEGIN_INANITY_GRAPHICS

Dx11VertexShader::Dx11VertexShader(ptr<File> code, ComPointer<ID3D11VertexShader> vertexShader, const Dx11ShaderResources& resources)
: code(code), vertexShader(vertexShader), resources(resources) {}

ID3D11VertexShader* Dx11VertexShader::GetVertexShaderInterface() const
{
	return vertexShader;
}

ptr<File> Dx11VertexShader::GetCode() const
{
	return code;
}

const Dx11ShaderResources& Dx11VertexShader::GetResources() const
{
	return resources;
}

END_INANITY_GRAPHICS
