#include "GlxPresenter.hpp"
#include "GlDevice.hpp"
#include "GlFrameBuffer.hpp"
#include "X11MonitorMode.hpp"
#include "X11Output.hpp"
#include "../platform/X11Window.hpp"
#include "../platform/X11Display.hpp"

BEGIN_INANITY_GRAPHICS

GlxPresenter::GlxPresenter(ptr<GlDevice> device, ptr<GlFrameBuffer> frameBuffer, ptr<X11Output> output, GLXWindow glxWindow)
: device(device), frameBuffer(frameBuffer), output(output), glxWindow(glxWindow)
{
	output->SetPresenter(this);
}

GlxPresenter::~GlxPresenter()
{
	output->SetPresenter(0);
}

ptr<Device> GlxPresenter::GetDevice() const
{
	return device;
}

ptr<FrameBuffer> GlxPresenter::GetFrameBuffer() const
{
	return frameBuffer;
}

void GlxPresenter::SetMode(ptr<MonitorMode> abstractMode)
{
	// TODO.
}

void GlxPresenter::Present()
{
	Platform::X11Window* window = output->GetWindow();
	glXSwapBuffers(window->GetDisplay()->GetDisplay(), glxWindow);
}

void GlxPresenter::Resize(int width, int height)
{
	// TODO.
}

END_INANITY_GRAPHICS
