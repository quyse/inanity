#include "GlPixelShader.hpp"
#include "GlShaderBindings.hpp"

GlPixelShader::GlPixelShader(GLuint shaderName, ptr<GlShaderBindings> shaderBindings)
: shaderName(shaderName), shaderBindings(shaderBindings) {}

GlPixelShader::~GlPixelShader()
{
	glDeleteShader(shaderName);
}

GLuint GlPixelShader::GetShaderName() const
{
	return shaderName;
}

ptr<GlShaderBindings> GlPixelShader::GetShaderBindings() const
{
	return shaderBindings;
}
