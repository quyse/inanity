#include "GlContext.hpp"

GlContext::GlContext() :
	targetsFramebuffer(0), serviceFramebuffer(0),
	currentFramebuffer(0), dirtyCurrentFramebuffer(true)
{
}

GlContext::~GlContext()
{
	if(targetsFramebuffer)
		glDeleteFramebuffers(1, &targetsFramebuffer);
	if(serviceFramebuffer)
		glDeleteFramebuffers(1, &serviceFramebuffer);
}

void GlContext::BindTargetsFramebuffer()
{
	if(!targetsFramebuffer)
		glGenFramebuffers(1, &targetsFramebuffer);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targetsFramebuffer);

	dirtyCurrentFramebuffer = true;
}

void GlContext::BindServiceFramebuffer()
{
	if(!serviceFramebuffer)
		glGenFramebuffers(1, &serviceFramebuffer);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, serviceFramebuffer);

	dirtyCurrentFramebuffer = true;
}

void GlContext::Update()
{
	// TODO
}

void GlContext::ClearRenderBuffer(RenderBuffer* renderBuffer, const float* color)
{
	BindServiceFramebuffer();
	glFramebufferRenderBuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderBuffer->GetName());
	glClearBufferfv(GL_COLOR, GL_DRAWBUFFER0, color);
}

void GlContext::ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, float depth)
{
	BindServiceFramebuffer();
	glFramebufferRenderBuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthStencilBuffer->GetName());
	glClearBufferfv(GL_DEPTH, 0, &depth);
}

void GlContext::ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, unsigned stencil)
{
	BindServiceFramebuffer();
	glFramebufferRenderBuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthStencilBuffer->GetName());
	GLint s = stencil;
	glClearBufferiv(GL_STENCIL, 0, &stencil);
}

void GlContext::ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, float depth, unsigned stencil)
{
	BindServiceFramebuffer();
	glFramebufferRenderBuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthStencilBuffer->GetName());
	glClearBufferfi(GL_DEPTH_STENCIL, 0, depth, stencil);
}

void GlContext::SetUniformBufferData(UniformBuffer* buffer, const void* data, size_t size)
{
	// TODO
}
