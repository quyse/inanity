#include "WglPresenter.hpp"
#include "GlDevice.hpp"
#include "GlFrameBuffer.hpp"
#include "Win32MonitorMode.hpp"
#include "../platform/Win32Window.hpp"

BEGIN_INANITY_GRAPHICS

WglPresenter::WglPresenter(ptr<GlDevice> device, ptr<GlFrameBuffer> frameBuffer, HDC hdc, ptr<Platform::Win32Window> window)
: GlPresenter(device, frameBuffer), hdc(hdc), window(window), currentSwapInterval(-1), targetSwapInterval(1)
{
	window->SetPresenter(this);
	width = window->GetClientWidth();
	height = window->GetClientHeight();
}

WglPresenter::~WglPresenter()
{
	window->SetPresenter(nullptr);
}

void WglPresenter::Bind(HGLRC hglrc)
{
	if(!wglMakeCurrent(hdc, hglrc))
		THROW_SECONDARY("Can't bind WGL presenter", Exception::SystemError());
}

int WglPresenter::GetWidth() const
{
	return width;
}

int WglPresenter::GetHeight() const
{
	return height;
}

void WglPresenter::SetMode(ptr<MonitorMode> abstractMode)
{
	BEGIN_TRY();

	// if mode is not fullscreen, nothing to do
	// restoring to first mode is not supported yet
	if(!abstractMode)
		return;

	ptr<Win32MonitorMode> mode = abstractMode.DynamicCast<Win32MonitorMode>();
	if(!mode && abstractMode)
		THROW("Only Win32 monitor mode allowed");

	// get monitor for window
	HMONITOR monitor = MonitorFromWindow(window->GetHWND(), MONITOR_DEFAULTTOPRIMARY);
	if(!monitor)
		THROW("Can't get window monitor");
	MONITORINFOEX monitorInfo;
	monitorInfo.cbSize = sizeof(monitorInfo);
	if(!GetMonitorInfo(monitor, &monitorInfo))
		THROW("Can't get monitor info");
	DEVMODE modeInfo = mode->GetInfo();
	if(ChangeDisplaySettingsEx(monitorInfo.szDevice, &modeInfo, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL)
		THROW("Can't change display settings");

	END_TRY("Can't set mode for WGL presenter");
}

void WglPresenter::SetSwapInterval(int swapInterval)
{
	targetSwapInterval = swapInterval;
}

void WglPresenter::Present()
{
	if(targetSwapInterval != currentSwapInterval)
	{
		if(WGLEW_EXT_swap_control) wglSwapIntervalEXT(targetSwapInterval);
		currentSwapInterval = targetSwapInterval;
	}
	SwapBuffers(hdc);
}

void WglPresenter::Resize(int width, int height)
{
	// remember size
	this->width = width;
	this->height = height;
}

END_INANITY_GRAPHICS
