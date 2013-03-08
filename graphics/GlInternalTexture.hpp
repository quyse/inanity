#ifndef ___INANITY_GRAPHICS_GL_INTERNAL_TEXTURE_HPP___
#define ___INANITY_GRAPHICS_GL_INTERNAL_TEXTURE_HPP___

#include "graphics.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

/// Объект, хранящий текстуру OpenGL.
/** Используется объектами GlRenderBuffer, GlDepthStencilBuffer и GlTexture,
так как все эти объекты реализованы через текстуры OpenGL. */
class GlInternalTexture : public Object
{
private:
	GLuint name;

public:
	GlInternalTexture(GLuint name);
	~GlInternalTexture();

	GLuint GetName() const;
};

END_INANITY_GRAPHICS

#endif
