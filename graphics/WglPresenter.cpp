#include "WglPresenter.hpp"
#include "GlDevice.hpp"
#include "GlFrameBuffer.hpp"
#include "Win32MonitorMode.hpp"

BEGIN_INANITY_GRAPHICS

WglPresenter::WglPresenter(ptr<GlDevice> device, ptr<GlFrameBuffer> frameBuffer, HDC hdc)
: device(device), frameBuffer(frameBuffer), hdc(hdc) {}

ptr<Device> WglPresenter::GetDevice() const
{
	return device;
}

ptr<FrameBuffer> WglPresenter::GetFrameBuffer() const
{
	return frameBuffer;
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
