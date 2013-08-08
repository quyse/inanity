#include "GlInternalTexture.hpp"
#include "GlDevice.hpp"

BEGIN_INANITY_GRAPHICS

GlInternalTexture::GlInternalTexture(ptr<GlDevice> device, GLuint name)
: device(device), name(name) {}

GlInternalTexture::~GlInternalTexture()
{
	glDeleteTextures(1, &name);
}

GLuint GlInternalTexture::GetName() const
{
	return name;
}

END_INANITY_GRAPHICS
