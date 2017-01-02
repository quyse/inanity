#include "EmsPresenter.hpp"
#include "GlDevice.hpp"
#include "GlFrameBuffer.hpp"
#include "../platform/EmsWindow.hpp"

BEGIN_INANITY_GRAPHICS

EmsPresenter::EmsPresenter(ptr<GlDevice> device, ptr<GlFrameBuffer> frameBuffer, ptr<Platform::EmsWindow> window)
: device(device), frameBuffer(frameBuffer), window(window)
{
	window->SetPresenter(this);
	width = window->GetWidth();
	height = window->GetHeight();
}

EmsPresenter::~EmsPresenter()
{
	window->SetPresenter(nullptr);
}

ptr<Device> EmsPresenter::GetDevice() const
{
	return device;
}

int EmsPresenter::GetWidth() const
{
	return width;
}

int EmsPresenter::GetHeight() const
{
	return height;
}

ptr<FrameBuffer> EmsPresenter::GetFrameBuffer() const
{
	return frameBuffer;
}

void EmsPresenter::SetMode(ptr<MonitorMode> mode)
{
}

void EmsPresenter::SetSwapInterval(int swapInterval)
{
}

void EmsPresenter::Present()
{
}

void EmsPresenter::Resize(int width, int height)
{
	this->width = width;
	this->height = height;
}

END_INANITY_GRAPHICS
