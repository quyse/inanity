#include "GlDepthStencilBuffer.hpp"
#include "GlInternalTexture.hpp"
#include "GlTexture.hpp"

GlDepthStencilBuffer::GlDepthStencilBuffer(ptr<GlInternalTexture> internalTexture, ptr<GlTexture> texture)
: internalTexture(internalTexture), texture(texture)
{
}

GLuint GlDepthStencilBuffer::GetName() const
{
	return internalTexture->GetName();
}

ptr<Texture> GlDepthStencilBuffer::GetTexture()
{
	return texture;
}
