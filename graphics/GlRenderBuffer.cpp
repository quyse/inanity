#include "GlRenderBuffer.hpp"
#include "GlTexture.hpp"

GlRenderBuffer::GlRenderBuffer(GLuint renderBufferName, ptr<GlTexture> texture)
: renderBufferName(renderBufferName), texture(texture)
{
}

~GlRenderBuffer::GlRenderBuffer()
{
	if(renderBufferName)
		glDeleteRenderBuffers(1, &renderBufferName);
}

ptr<Texture> GlRenderBuffer::GetTexture()
{
	return texture;
}
