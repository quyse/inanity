#ifndef ___INANITY_GL_TEXTURE_HPP___
#define ___INANITY_GL_TEXTURE_HPP___

#include "Texture.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

class GlInternalTexture;

/// Класс текстуры OpenGL.
class GlTexture : public Texture
{
private:
	ptr<GlInternalTexture> internalTexture;

public:
	GlTexture(ptr<GlInternalTexture> internalTexture, int width, int height, int depth);

	GLuint GetName() const;
};

END_INANITY_GRAPHICS

#endif
