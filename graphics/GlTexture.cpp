#include "GlTexture.hpp"
#include "GlInternalTexture.hpp"
#include "opengl.hpp"

GlTexture::GlTexture(ptr<GlInternalTexture> internalTexture) : internalTexture(internalTexture) {}

GLuint GlTexture::GetName() const
{
	return internalTexture->GetName();
}
