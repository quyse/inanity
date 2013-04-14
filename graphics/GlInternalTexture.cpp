#include "GlInternalTexture.hpp"

BEGIN_INANITY_GRAPHICS

GlInternalTexture::GlInternalTexture(GLuint name) : name(name) {}

GlInternalTexture::~GlInternalTexture()
{
	glDeleteTextures(1, &name);
}

GLuint GlInternalTexture::GetName() const
{
	return name;
}

END_INANITY_GRAPHICS
