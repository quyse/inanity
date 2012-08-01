#include "GlVertexShader.hpp"

GlVertexShader::GlVertexShader(GLuint programName, GLuint shaderName)
: programName(programName), shaderName(shaderName) {}

GlVertexShader::~GlVertexShader()
{
	glDeleteProgram(programName);
	glDeleteShader(shaderName);
}

GLuint GlVertexShader::GetProgramName() const
{
	return programName;
}
