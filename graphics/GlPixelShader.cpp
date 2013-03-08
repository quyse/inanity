#include "GlPixelShader.hpp"

GlPixelShader::GlPixelShader(GLuint shaderName) : shaderName(shaderName) {}

GlPixelShader::~GlPixelShader()
{
	glDeleteShader(shaderName);
}

GLuint GlPixelShader::GetShaderName() const
{
	return shaderName;
}
