#include "SdlPresenter.hpp"
#include "GlDevice.hpp"
#include "GlFrameBuffer.hpp"

BEGIN_INANITY_GRAPHICS

SdlPresenter::SdlPresenter(ptr<GlDevice> device, ptr<GlFrameBuffer> frameBuffer, ptr<Platform::SdlWindow> window)
: device(device), frameBuffer(frameBuffer), window(window), currentSwapInterval(-1), targetSwapInterval(1)
{
	window->SetPresenter(this);
	width = window->GetClientWidth();
	height = window->GetClientHeight();
}

SdlPresenter::~SdlPresenter()
{
	window->SetPresenter(nullptr);
	device->UnbindPresenter(this);
}

void SdlPresenter::Bind(SDL_GLContext sdlContext)
{
	SDL_GL_MakeCurrent(window->GetHandle(), sdlContext);
}

ptr<Device> SdlPresenter::GetDevice() const
{
	return device;
}

ptr<FrameBuffer> SdlPresenter::GetFrameBuffer() const
{
	return frameBuffer;
}

int SdlPresenter::GetWidth() const
{
	return width;
}

int SdlPresenter::GetHeight() const
{
	return height;
}

void SdlPresenter::SetMode(ptr<MonitorMode> mode)
{
}

void SdlPresenter::SetSwapInterval(int swapInterval)
{
	targetSwapInterval = swapInterval;
}

void SdlPresenter::Present()
{
	if(targetSwapInterval != currentSwapInterval)
	{
		SDL_GL_SetSwapInterval(targetSwapInterval);
		currentSwapInterval = targetSwapInterval;
	}
	SDL_GL_SwapWindow(window->GetHandle());
}

void SdlPresenter::Resize(int width, int height)
{
	this->width = width;
	this->height = height;
}

END_INANITY_GRAPHICS
