#include "GlInternalProgram.hpp"
#include "GlDevice.hpp"

BEGIN_INANITY_GRAPHICS

GlInternalProgram::GlInternalProgram(ptr<GlDevice> device, GLuint name)
: device(device), name(name) {}

GlInternalProgram::~GlInternalProgram()
{
	glDeleteProgram(name);
}

GLuint GlInternalProgram::GetName() const
{
	return name;
}

END_INANITY_GRAPHICS
