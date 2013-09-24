#ifndef ___INANITY_GRAPHICS_GL_FRAME_BUFFER_HPP___
#define ___INANITY_GRAPHICS_GL_FRAME_BUFFER_HPP___

#include "FrameBuffer.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

class GlDevice;

/// Frame buffer class for OpenGL graphics.
/** Default framebuffer is represented naturally
- with zero name and zero attachments. */
class GlFrameBuffer : public FrameBuffer
{
private:
	ptr<GlDevice> device;
	GLuint name;

public:
	GlFrameBuffer(ptr<GlDevice> device, GLuint name);
	~GlFrameBuffer();

	GLuint GetName() const;

	/// Apply framebuffer to OpenGL context.
	void Apply();
};

END_INANITY_GRAPHICS

#endif
