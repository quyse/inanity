#include "SdlPresenter.hpp"
#include "GlDevice.hpp"
#include "GlFrameBuffer.hpp"
#include "../platform/SdlWindow.hpp"
#include <SDL2/SDL_video.h>

BEGIN_INANITY_GRAPHICS

SdlPresenter::SdlPresenter(ptr<GlDevice> device, ptr<GlFrameBuffer> frameBuffer, ptr<Platform::SdlWindow> window)
: device(device), frameBuffer(frameBuffer), window(window)
{
	window->SetPresenter(this);
}

SdlPresenter::~SdlPresenter()
{
	window->SetPresenter(nullptr);
}

ptr<Device> SdlPresenter::GetDevice() const
{
	return device;
}

ptr<FrameBuffer> SdlPresenter::GetFrameBuffer() const
{
	return frameBuffer;
}

void SdlPresenter::SetMode(ptr<MonitorMode> mode)
{
}

void SdlPresenter::Present()
{
	SDL_GL_SwapWindow(window->GetHandle());
}

void SdlPresenter::Resize(int width, int height)
{
	// do nothing
}

END_INANITY_GRAPHICS
