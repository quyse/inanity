#include "DxVertexShader.hpp"

DxVertexShader::DxVertexShader(ID3D11VertexShader* vertexShader)
: vertexShader(vertexShader)
{
}

ID3D11VertexShader* DxVertexShader::GetVertexShaderInterface() const
{
	return vertexShader;
}
