#include "GlPixelShader.hpp"
#include "GlDevice.hpp"
#include "GlShaderBindings.hpp"

BEGIN_INANITY_GRAPHICS

GlPixelShader::GlPixelShader(ptr<GlDevice> device, GLuint shaderName, ptr<GlShaderBindings> shaderBindings)
: device(device), shaderName(shaderName), shaderBindings(shaderBindings) {}

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
