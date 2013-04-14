#include "GlInternalProgram.hpp"

BEGIN_INANITY_GRAPHICS

GlInternalProgram::GlInternalProgram(GLuint name)
: name(name) {}

GlInternalProgram::~GlInternalProgram()
{
	glDeleteProgram(name);
}

GLuint GlInternalProgram::GetName() const
{
	return name;
}

END_INANITY_GRAPHICS
