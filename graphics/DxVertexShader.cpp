#include "DxVertexShader.hpp"
#include "Layout.hpp"
#include "../File.hpp"

DxVertexShader::DxVertexShader(ptr<File> code, ID3D11VertexShader* vertexShader)
: code(code), vertexShader(vertexShader)
{
}

ptr<Layout> DxVertexShader::GetAttributesLayout()
{
	// TODO
	return 0;
}

ID3D11VertexShader* DxVertexShader::GetVertexShaderInterface() const
{
	return vertexShader;
}

ptr<File> DxVertexShader::GetCode() const
{
	return code;
}
