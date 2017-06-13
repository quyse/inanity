#include "GlFrameBuffer.hpp"
#include "GlSystem.hpp"
#include "GlDevice.hpp"
#include "GlRenderBuffer.hpp"
#include "GlDepthStencilBuffer.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

GlFrameBuffer::GlFrameBuffer(ptr<GlDevice> device, GLuint name)
: device(device), name(name), presenter(nullptr) {}

GlFrameBuffer::~GlFrameBuffer()
{
	if(name)
		glDeleteFramebuffers(1, &name);
}

GLuint GlFrameBuffer::GetName() const
{
	return name;
}

void GlFrameBuffer::SetPresenter(Presenter* presenter)
{
	this->presenter = presenter;
}

void GlFrameBuffer::Apply()
{
	BEGIN_TRY();

	if(presenter)
		device->BindPresenter(presenter);

	// bind framebuffer
#ifdef ___INANITY_PLATFORM_EMSCRIPTEN
	glBindFramebuffer(GL_FRAMEBUFFER, name);
#else
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, name);
#endif
	GlSystem::CheckErrors("Can't bind framebuffer");

	// if not dirty, all ok
	if(!dirty)
		return;

	int maxColorBuffersCount = device->GetCaps().maxColorBuffersCount;

	// check if the framebuffer is default
	if(name == 0)
	{
		// check that no buffers is bound
		for(int i = 0; i < maxColorBuffersCount; ++i)
			if(colorBuffers[i])
				THROW("No color buffers can be bound to default framebuffer");
		if(depthStencilBuffer)
			THROW("No depth-stencil buffer can be bound to default framebuffer");
	}
	else
	{
		// bind color buffers
		for(int i = 0; i < maxColorBuffersCount; ++i)
		{
			RenderBuffer* abstractRenderBuffer = colorBuffers[i];
			if(abstractRenderBuffer)
			{
				GLuint bufferName = fast_cast<GlRenderBuffer*>(abstractRenderBuffer)->GetName();
				if(bufferName == 0)
					THROW("Default renderbuffer can't be bound with other buffers");
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, bufferName, 0);
			}
			else if(i == 0)
				THROW("Zero color attachment should be set");
			else
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, 0, 0);
		}

		// bind depth-stencil buffer
		DepthStencilBuffer* abstractDepthStencilBuffer = depthStencilBuffer;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D,
			abstractDepthStencilBuffer ? fast_cast<GlDepthStencilBuffer*>(abstractDepthStencilBuffer)->GetName() : 0, 0);

		GlSystem::CheckErrors("Can't bind some buffers");
	}

	dirty = false;

	END_TRY("Can't apply OpenGL frame buffer");
}

END_INANITY_GRAPHICS
