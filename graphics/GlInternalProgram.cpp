#include "GlInternalProgram.hpp"
#include "GlDevice.hpp"

BEGIN_INANITY_GRAPHICS

GlInternalProgram::GlInternalProgram(ptr<GlDevice> device, GLuint name, const UniformBindings& uniformBindings)
: device(device), name(name), uniformBindings(uniformBindings) {}

GlInternalProgram::~GlInternalProgram()
{
	glDeleteProgram(name);
}

GLuint GlInternalProgram::GetName() const
{
	return name;
}

const GlInternalProgram::UniformBindings& GlInternalProgram::GetUniformBindings() const
{
	return uniformBindings;
}

END_INANITY_GRAPHICS
