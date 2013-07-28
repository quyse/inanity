#include "GlVertexShader.hpp"
#include "GlDevice.hpp"
#include "GlShaderBindings.hpp"

BEGIN_INANITY_GRAPHICS

GlVertexShader::GlVertexShader(ptr<GlDevice> device, GLuint shaderName, ptr<GlShaderBindings> shaderBindings)
: device(device), shaderName(shaderName), shaderBindings(shaderBindings) {}

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
