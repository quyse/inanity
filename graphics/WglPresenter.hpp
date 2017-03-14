#ifndef ___INANITY_GRAPHICS_WGL_PRESENTER_HPP___
#define ___INANITY_GRAPHICS_WGL_PRESENTER_HPP___

#include "GlPresenter.hpp"
#include "../platform/platform.hpp"
#include "../platform/windows.hpp"

BEGIN_INANITY_PLATFORM

class Win32Window;

END_INANITY_PLATFORM

BEGIN_INANITY_GRAPHICS

/// Presenter class for OpenGL on Windows.
class WglPresenter : public GlPresenter
{
private:
	HDC hdc;
	ptr<Platform::Win32Window> window;
	int width, height;
	int currentSwapInterval, targetSwapInterval;

public:
	WglPresenter(ptr<GlDevice> device, ptr<GlFrameBuffer> frameBuffer, HDC hdc, ptr<Platform::Win32Window> window);
	~WglPresenter();

	void Bind(HGLRC hglrc);

	// Presenter's methods.
	int GetWidth() const;
	int GetHeight() const;
	void SetMode(ptr<MonitorMode> mode);
	void SetSwapInterval(int swapInterval);
	void Present();
	void Resize(int width, int height);
};

END_INANITY_GRAPHICS

#endif
