#include "GlSamplerState.hpp"
#include "GlDevice.hpp"
#include "GlSystem.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

GlSamplerState::GlSamplerState(ptr<GlDevice> device, GLuint name)
: device(device), name(name)
{}

GlSamplerState::~GlSamplerState()
{
	glDeleteSamplers(1, &name);
}

GLuint GlSamplerState::GetName()
{
	return name;
}

END_INANITY_GRAPHICS
