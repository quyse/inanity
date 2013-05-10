#include "DxVertexShader.hpp"
#include "../File.hpp"

BEGIN_INANITY_GRAPHICS

DxVertexShader::DxVertexShader(ptr<File> code, ComPointer<ID3D11VertexShader> vertexShader, const DxShaderResources& resources)
: code(code), vertexShader(vertexShader), resources(resources) {}

ID3D11VertexShader* DxVertexShader::GetVertexShaderInterface() const
{
	return vertexShader;
}

ptr<File> DxVertexShader::GetCode() const
{
	return code;
}

const DxShaderResources& DxVertexShader::GetResources() const
{
	return resources;
}

END_INANITY_GRAPHICS
