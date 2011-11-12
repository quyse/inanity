#ifndef ___INANITY_DX_CONTEXT_HPP___
#define ___INANITY_DX_CONTEXT_HPP___

#include "dx.hpp"
#include "d3d.hpp"
#include "../../ComPointer.hpp"
#include <vector>

BEGIN_INANITY_DX

class RenderBuffer;
class DepthStencilBuffer;

class Context : public Object
{
private:
	ComPointer<ID3D11DeviceContext> context;
	size_t lastViewportWidth, lastViewportHeight;
	std::vector<ID3D11RenderTargetView*> renderTargets;

public:
	/// Создать графический контекст.
	Context(ID3D11DeviceContext* context);

	ID3D11DeviceContext* GetDeviceContext() const;

	void SetViewport(size_t width, size_t height);

	/// Очистить рендербуфер.
	void ClearRenderBuffer(RenderBuffer* renderBuffer, const float* color);
	/// Очистить глубину в depth-stencil буфере.
	void ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, float depth);
	/// Очистить трафарет в depth-stencil буфере.
	void ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, unsigned stencil);
	/// Очистить глубину и трафарет в depth-stencil буфере.
	void ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, float depth, unsigned stencil);

	/// Указать цели для рендеринга.
	void SetRenderTargets(const std::vector<ptr<RenderBuffer> >& renderBuffers, ptr<DepthStencilBuffer> depthStencilBuffer);
};

END_INANITY_DX

#endif
