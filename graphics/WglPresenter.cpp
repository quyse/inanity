#include "WglPresenter.hpp"
#include "GlDevice.hpp"
#include "GlRenderBuffer.hpp"
#include "Win32MonitorMode.hpp"

BEGIN_INANITY_GRAPHICS

WglPresenter::WglPresenter(ptr<GlDevice> device, ptr<GlRenderBuffer> backBuffer, HDC hdc)
: device(device), backBuffer(backBuffer), hdc(hdc) {}

ptr<Device> WglPresenter::GetDevice()
{
	return device;
}

ptr<RenderBuffer> WglPresenter::GetBackBuffer()
{
	return backBuffer;
}

void WglPresenter::SetMode(ptr<MonitorMode> abstractMode)
{
	// TODO.
}

void WglPresenter::Present()
{
	SwapBuffers(hdc);
}

void WglPresenter::Resize(int width, int height)
{
	// TODO.
}

END_INANITY_GRAPHICS
