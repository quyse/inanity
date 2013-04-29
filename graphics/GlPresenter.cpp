#include "GlPresenter.hpp"
#include "GlDevice.hpp"
#include "GlRenderBuffer.hpp"

BEGIN_INANITY_GRAPHICS

#ifdef ___INANITY_WINDOWS
GlPresenter::GlPresenter(ptr<GlDevice> device, HDC hdc, ptr<GlRenderBuffer> backBuffer)
: device(device), hdc(hdc), backBuffer(backBuffer) {}
#endif

#ifdef ___INANITY_LINUX
GlPresenter::GlPresenter(ptr<GlDevice> device, ptr<GlRenderBuffer> backBuffer)
: device(device), backBuffer(backBuffer) {}
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
	// TODO.
}

void GlPresenter::Present()
{
#ifdef ___INANITY_WINDOWS
	SwapBuffers(hdc);
#endif
}

void GlPresenter::Resize(int width, int height)
{
	// TODO.
}

END_INANITY_GRAPHICS
