#include "NxPresenter.hpp"
#include "GlDevice.hpp"
#include "GlPresenter.hpp"
#include "GlFrameBuffer.hpp"
#include "../platform/NxWindow.hpp"

BEGIN_INANITY_GRAPHICS

NxPresenter::NxPresenter(ptr<GlDevice> device, ptr<GlFrameBuffer> frameBuffer, ptr<Platform::NxWindow> window)
: GlPresenter(device, frameBuffer), window(window)
{
}

int NxPresenter::GetWidth() const
{
	return 0; // TODO
}

int NxPresenter::GetHeight() const
{
	return 0; // TODO
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
	// TODO
}

void NxPresenter::Resize(int width, int height)
{
	// TODO
}

END_INANITY_GRAPHICS
