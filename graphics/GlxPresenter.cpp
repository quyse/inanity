#include "GlxPresenter.hpp"
#include "GlDevice.hpp"
#include "GlRenderBuffer.hpp"
#include "X11MonitorMode.hpp"
#include "X11Output.hpp"
#include "../platform/X11Window.hpp"
#include "../platform/X11Display.hpp"

BEGIN_INANITY_GRAPHICS

GlxPresenter::GlxPresenter(ptr<GlDevice> device, ptr<GlRenderBuffer> backBuffer, ptr<X11Output> output, GLXWindow glxWindow)
: device(device), backBuffer(backBuffer), output(output), glxWindow(glxWindow)
{
	output->SetPresenter(this);
}

GlxPresenter::~GlxPresenter()
{
	output->SetPresenter(0);
}

ptr<Device> GlxPresenter::GetDevice()
{
	return device;
}

ptr<RenderBuffer> GlxPresenter::GetBackBuffer()
{
	return backBuffer;
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
