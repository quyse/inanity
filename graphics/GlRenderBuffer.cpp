#include "GlRenderBuffer.hpp"
#include "GlTexture.hpp"
#include "opengl.hpp"

GlRenderBuffer::GlRenderBuffer(GLuint name, ptr<GlTexture> texture)
: name(name), texture(texture)
{
}

~GlRenderBuffer::GlRenderBuffer()
{
	if(name)
		glDeleteRenderBuffers(1, &name);
}

GLuint GlRenderBuffer::GetName() const
{
	return name;
}

ptr<Texture> GlRenderBuffer::GetTexture()
{
	return texture;
}
