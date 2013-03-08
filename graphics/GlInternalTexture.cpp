#include "GlInternalTexture.hpp"

GlInternalTexture::GlInternalTexture(GLuint name) : name(name) {}

GlInternalTexture::~GlInternalTexture()
{
	glDeleteTextures(1, &name);
}

GLuint GlInternalTexture::GetName() const
{
	return name;
}
