#include "GlDepthStencilBuffer.hpp"
#include "GlInternalTexture.hpp"
#include "GlTexture.hpp"

GlDepthStencilBuffer::GlDepthStencilBuffer(ptr<GlInternalTexture> internalTexture, ptr<GlTexture> texture, int width, int height)
: internalTexture(internalTexture), texture(texture), width(width), height(height) {}

GLuint GlDepthStencilBuffer::GetName() const
{
	return internalTexture->GetName();
}

int GlDepthStencilBuffer::GetWidth() const
{
	return width;
}

int GlDepthStencilBuffer::GetHeight() const
{
	return height;
}

ptr<Texture> GlDepthStencilBuffer::GetTexture()
{
	return texture;
}
