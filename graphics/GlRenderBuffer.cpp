#include "GlRenderBuffer.hpp"
#include "GlInternalTexture.hpp"
#include "GlTexture.hpp"

BEGIN_INANITY_GRAPHICS

GlRenderBuffer::GlRenderBuffer(ptr<GlInternalTexture> internalTexture, ptr<GlTexture> texture)
: internalTexture(internalTexture), texture(texture)
{
}

GLuint GlRenderBuffer::GetName() const
{
	return internalTexture ? internalTexture->GetName() : 0;
}

ptr<Texture> GlRenderBuffer::GetTexture()
{
	return texture;
}

float GlRenderBuffer::GetScreenTopTexcoord() const
{
	return 1;
}

END_INANITY_GRAPHICS
