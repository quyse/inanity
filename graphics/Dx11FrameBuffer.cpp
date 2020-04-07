#include "Dx11FrameBuffer.hpp"
#include "Dx11Device.hpp"
#include "Dx11Presenter.hpp"
#include "Dx11RenderBuffer.hpp"
#include "Dx11DepthStencilBuffer.hpp"

BEGIN_INANITY_GRAPHICS

Dx11FrameBuffer::Dx11FrameBuffer(ptr<Dx11Device> device, Dx11Presenter* presenter)
: device(device), presenter(presenter) {}

Dx11RenderBuffer* Dx11FrameBuffer::GetColorBuffer(int i) const
{
	return (i || !presenter)
		? colorBuffers[i].FastCast<Dx11RenderBuffer>()
		: presenter->GetBackBuffer();
}

void Dx11FrameBuffer::Apply(ID3D11DeviceContext* deviceContext)
{
	ID3D11RenderTargetView* views[maxColorBuffersCount];
	for(int i = 0; i < maxColorBuffersCount; ++i)
	{
		RenderBuffer* abstractColorBuffer = colorBuffers[i];
		views[i] = abstractColorBuffer ?
			fast_cast<Dx11RenderBuffer*>(abstractColorBuffer)->GetRenderTargetViewInterface() : nullptr;
	}

	if(presenter)
		views[0] = presenter->GetBackBuffer()->GetRenderTargetViewInterface();

	ID3D11DepthStencilView* depthStencilView = depthStencilBuffer ?
		fast_cast<Dx11DepthStencilBuffer*>(&*depthStencilBuffer)->GetDepthStencilViewInterface() : nullptr;

	// apply to context
	deviceContext->OMSetRenderTargets(maxColorBuffersCount, views, depthStencilView);

	// clear dirty flag
	dirty = false;
}

END_INANITY_GRAPHICS
