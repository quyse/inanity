#include "GlVertexShader.hpp"

GlVertexShader::GlVertexShader(GLuint shaderName) : shaderName(shaderName) {}

GlVertexShader::~GlVertexShader()
{
	glDeleteShader(shaderName);
}

GLuint GlVertexShader::GetShaderName() const
{
	return shaderName;
}
