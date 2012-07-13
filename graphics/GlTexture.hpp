#ifndef ___INANITY_GL_TEXTURE_HPP___
#define ___INANITY_GL_TEXTURE_HPP___

#include "Texture.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс текстуры OpenGL.
class GlTexture : public Texture
{
private:
	GLuint textureName;

public:
	GlTexture(GLuint textureName);
	~GlTexture();
};

END_INANITY_GRAPHICS

#endif
