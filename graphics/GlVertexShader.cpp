#include "GlVertexShader.hpp"
#include "GlShaderBindings.hpp"

GlVertexShader::GlVertexShader(GLuint shaderName, ptr<GlShaderBindings> shaderBindings)
: shaderName(shaderName), shaderBindings(shaderBindings) {}

GlVertexShader::~GlVertexShader()
{
	glDeleteShader(shaderName);
}

GLuint GlVertexShader::GetShaderName() const
{
	return shaderName;
}

ptr<GlShaderBindings> GlVertexShader::GetShaderBindings() const
{
	return shaderBindings;
}
