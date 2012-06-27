#include "Shader.hpp"
#include "ShaderReflection.hpp"

Shader::Shader(ptr<ShaderReflection> shaderReflection) : shaderReflection(shaderReflection)
{
}

ptr<ShaderReflection> Shader::GetReflection() const
{
	return shaderReflection;
}
