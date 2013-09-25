#ifndef ___INANITY_GRAPHICS_DX11_FRAME_BUFFER_HPP___
#define ___INANITY_GRAPHICS_DX11_FRAME_BUFFER_HPP___

#include "FrameBuffer.hpp"
#include "d3d11.hpp"

BEGIN_INANITY_GRAPHICS

class Dx11Device;
class Dx11Presenter;
class Dx11RenderBuffer;

/// Framebuffer class for DirectX 11.
class Dx11FrameBuffer : public FrameBuffer
{
private:
	ptr<Dx11Device> device;
	/// Presenter to get back buffer into first slot.
	/** If null, this is not a default framebuffer. */
	Dx11Presenter* presenter;

public:
	Dx11FrameBuffer(ptr<Dx11Device> device, Dx11Presenter* presenter = 0);

	/// Get color buffer.
	/** Overloaded to return backbuffer when it's a default framebuffer. */
	Dx11RenderBuffer* GetColorBuffer(int i) const;

	/// Apply framebuffer to context.
	void Apply(ID3D11DeviceContext* deviceContext);
};

END_INANITY_GRAPHICS

#endif
