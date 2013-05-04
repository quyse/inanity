#include "DxVertexShader.hpp"
#include "../File.hpp"

BEGIN_INANITY_GRAPHICS

DxVertexShader::DxVertexShader(ptr<File> code, ComPointer<ID3D11VertexShader> vertexShader)
: code(code), vertexShader(vertexShader) {}

ID3D11VertexShader* DxVertexShader::GetVertexShaderInterface() const
{
	return vertexShader;
}

ptr<File> DxVertexShader::GetCode() const
{
	return code;
}

END_INANITY_GRAPHICS
