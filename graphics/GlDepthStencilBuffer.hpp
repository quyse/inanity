#ifndef ___INANITY_GRAPHICS_GL_DEPTH_STENCIL_BUFFER_HPP___
#define ___INANITY_GRAPHICS_GL_DEPTH_STENCIL_BUFFER_HPP___

#include "DepthStencilBuffer.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс буфера depth-stencil для OpenGL.
class GlDepthStencilBuffer : public DepthStencilBuffer
{
private:
	ptr<GlDevice> device;
	GLuint name;

public:
	GlDepthStencilBuffer(ptr<GlDevice> device, GLuint name);

	GLuint GetName() const;

	// методы DepthStencilBuffer
	ptr<Texture> GetTexture();
};

END_INANITY_GRAPHICS

#endif
