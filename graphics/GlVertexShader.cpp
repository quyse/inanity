#include "GlVertexShader.hpp"
#include "GlShaderBindings.hpp"

BEGIN_INANITY_GRAPHICS

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

END_INANITY_GRAPHICS
