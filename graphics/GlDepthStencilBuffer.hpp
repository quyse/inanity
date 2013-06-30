#ifndef ___INANITY_GRAPHICS_GL_DEPTH_STENCIL_BUFFER_HPP___
#define ___INANITY_GRAPHICS_GL_DEPTH_STENCIL_BUFFER_HPP___

#include "DepthStencilBuffer.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

class GlInternalTexture;
class GlTexture;

/// Класс буфера depth-stencil для OpenGL.
class GlDepthStencilBuffer : public DepthStencilBuffer
{
private:
	ptr<GlInternalTexture> internalTexture;
	ptr<GlTexture> texture;
	int width, height;

public:
	GlDepthStencilBuffer(ptr<GlInternalTexture> internalTexture, ptr<GlTexture> texture, int width, int height);

	GLuint GetName() const;
	int GetWidth() const;
	int GetHeight() const;

	// методы DepthStencilBuffer
	ptr<Texture> GetTexture();
};

END_INANITY_GRAPHICS

#endif
