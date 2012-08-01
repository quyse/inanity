#include "GlPixelShader.hpp"

GlPixelShader::GlPixelShader(GLuint programName, GLuint shaderName)
: programName(programName), shaderName(shaderName) {}

GlPixelShader::~GlPixelShader()
{
	glDeleteProgram(programName);
	glDeleteShader(shaderName);
}

GLuint GlPixelShader::GetProgramName() const
{
	return programName;
}
