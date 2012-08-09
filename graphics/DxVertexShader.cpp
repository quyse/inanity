#include "DxVertexShader.hpp"
#include "../File.hpp"

DxVertexShader::DxVertexShader(ptr<File> code, ID3D11VertexShader* vertexShader)
: code(code), vertexShader(vertexShader)
{
}

ID3D11VertexShader* DxVertexShader::GetVertexShaderInterface() const
{
	return vertexShader;
}

ptr<File> DxVertexShader::GetCode() const
{
	return code;
}
