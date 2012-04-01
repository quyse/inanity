#include "VertexShader.hpp"
#include "ShaderReflection.hpp"

VertexShader::VertexShader(ptr<ShaderReflection> shaderReflection, ID3D11VertexShader* vertexShader)
: Shader(shaderReflection), vertexShader(vertexShader)
{
}

ID3D11VertexShader* VertexShader::GetInternalInterface() const
{
	return vertexShader;
}
