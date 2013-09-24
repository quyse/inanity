#ifndef ___INANITY_GRAPHICS_FRAME_BUFFER_HPP___
#define ___INANITY_GRAPHICS_FRAME_BUFFER_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

class RenderBuffer;
class DepthStencilBuffer;

/// Abstract class of graphics frame buffer.
/** Containts color & depth targets. */
class FrameBuffer : public Object
{
public:
	/// Maximum number of color buffers.
	static const int maxColorBuffersCount = 8;

protected:
	/// Color buffers.
	ptr<RenderBuffer> colorBuffers[maxColorBuffersCount];
	/// Depth-stencil buffer (optional).
	ptr<DepthStencilBuffer> depthStencilBuffer;
	/// If the buffer is changed after last time.
	bool dirty;

public:
	FrameBuffer();

	bool IsDirty() const;

	ptr<RenderBuffer> GetColorBuffer(int i) const;
	void SetColorBuffer(int i, ptr<RenderBuffer> colorBuffer);

	ptr<DepthStencilBuffer> GetDepthStencilBuffer() const;
	void SetDepthStencilBuffer(ptr<DepthStencilBuffer> depthStencilBuffer);
};

END_INANITY_GRAPHICS

#endif
