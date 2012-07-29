#include "GlTexture.hpp"
#include "opengl.hpp"

GlTexture::GlTexture(GLuint textureName) : textureName(textureName)
{
}

GlTexture::GlTexture()
{
	glDeleteTextures(1, &textureName);
}
