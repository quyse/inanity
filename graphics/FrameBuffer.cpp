#include "FrameBuffer.hpp"
#include "RenderBuffer.hpp"
#include "DepthStencilBuffer.hpp"

BEGIN_INANITY_GRAPHICS

FrameBuffer::FrameBuffer() : dirty(true) {}

bool FrameBuffer::IsDirty() const
{
	return dirty;
}

ptr<RenderBuffer> FrameBuffer::GetColorBuffer(int i) const
{
	return colorBuffers[i];
}

void FrameBuffer::SetColorBuffer(int i, ptr<RenderBuffer> colorBuffer)
{
	colorBuffers[i] = colorBuffer;
	dirty = true;
}

ptr<DepthStencilBuffer> FrameBuffer::GetDepthStencilBuffer() const
{
	return depthStencilBuffer;
}

void FrameBuffer::SetDepthStencilBuffer(ptr<DepthStencilBuffer> depthStencilBuffer)
{
	this->depthStencilBuffer = depthStencilBuffer;
	dirty = true;
}

END_INANITY_GRAPHICS
