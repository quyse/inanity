#include "WglPresenter.hpp"
#include "GlDevice.hpp"
#include "GlFrameBuffer.hpp"
#include "Win32MonitorMode.hpp"
#include "../platform/Win32Window.hpp"

BEGIN_INANITY_GRAPHICS

WglPresenter::WglPresenter(ptr<GlDevice> device, ptr<GlFrameBuffer> frameBuffer, HDC hdc, ptr<Platform::Win32Window> window)
: device(device), frameBuffer(frameBuffer), hdc(hdc), window(window)
{
	window->SetPresenter(this);
	width = window->GetClientWidth();
	height = window->GetClientHeight();
}

WglPresenter::~WglPresenter()
{
	window->SetPresenter(nullptr);
}

ptr<Device> WglPresenter::GetDevice() const
{
	return device;
}

int WglPresenter::GetWidth() const
{
	return width;
}

int WglPresenter::GetHeight() const
{
	return height;
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
	// remember size
	this->width = width;
	this->height = height;
}

END_INANITY_GRAPHICS
