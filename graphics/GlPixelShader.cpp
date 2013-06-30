#include "GlPixelShader.hpp"
#include "GlShaderBindings.hpp"

BEGIN_INANITY_GRAPHICS

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

END_INANITY_GRAPHICS
