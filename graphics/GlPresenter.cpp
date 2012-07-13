#include "GlPresenter.hpp"
#include "GlDevice.hpp"
#include "GlRenderBuffer.hpp"

#ifdef ___INANITY_WINDOWS
GlPresenter::GlPresenter(ptr<GlDevice> device, HDC hdc, ptr<RenderBuffer> backBuffer)
: device(device), hdc(hdc), backBuffer(backBuffer)
{
}
#endif

ptr<Device> GlPresenter::GetDevice()
{
	return device;
}

ptr<RenderBuffer> GlPresenter::GetBackBuffer()
{
	return backBuffer;
}

void GlPresenter::SetMode(const PresentMode& mode)
{
#error Not implemented yet.
}

void GlPresenter::Present()
{
#ifdef ___INANITY_WINDOWS
	SwapBuffers(hdc);
#endif
}
