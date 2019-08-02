#include "NxPresenter.hpp"
#include "GlDevice.hpp"
#include "GlPresenter.hpp"
#include "GlFrameBuffer.hpp"
#include "../platform/NxWindow.hpp"

BEGIN_INANITY_GRAPHICS

NxPresenter::NxPresenter(ptr<GlDevice> device, ptr<GlFrameBuffer> frameBuffer, ptr<Platform::NxWindow> window)
: GlPresenter(device, frameBuffer), window(window)
{
	width = window->GetClientWidth();
	height = window->GetClientHeight();
}

int NxPresenter::GetWidth() const
{
	return width;
}

int NxPresenter::GetHeight() const
{
	return height;
}

void NxPresenter::SetMode(ptr<MonitorMode> mode)
{
	// TODO
}

void NxPresenter::SetSwapInterval(int swapInterval)
{
	// TODO
}

void NxPresenter::Present()
{
	device->Present();
}

void NxPresenter::Resize(int width, int height)
{
	this->width = width;
	this->height = height;
}

END_INANITY_GRAPHICS
