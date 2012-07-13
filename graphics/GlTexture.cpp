#include "GlTexture.hpp"

GlTexture::GlTexture(GLuint textureName) : textureName(textureName)
{
}

GlTexture::GlTexture()
{
	glDeleteTextures(1, &textureName);
}
