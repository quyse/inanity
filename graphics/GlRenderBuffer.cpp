#include "GlRenderBuffer.hpp"
#include "GlInternalTexture.hpp"
#include "GlTexture.hpp"

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
