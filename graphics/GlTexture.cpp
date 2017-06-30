#include "GlTexture.hpp"
#include "GlInternalTexture.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

GlTexture::GlTexture(ptr<GlInternalTexture> internalTexture, int width, int height, int depth)
: Texture(width, height, depth), internalTexture(internalTexture) {}

GLuint GlTexture::GetName() const
{
	return internalTexture->GetName();
}

END_INANITY_GRAPHICS
