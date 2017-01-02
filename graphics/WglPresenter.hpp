#ifndef ___INANITY_GRAPHICS_WGL_PRESENTER_HPP___
#define ___INANITY_GRAPHICS_WGL_PRESENTER_HPP___

#include "Presenter.hpp"
#include "../platform/platform.hpp"
#include "../platform/windows.hpp"

BEGIN_INANITY_PLATFORM

class Win32Window;

END_INANITY_PLATFORM

BEGIN_INANITY_GRAPHICS

class GlDevice;
class GlFrameBuffer;

/// Presenter class for OpenGL on Windows.
class WglPresenter : public Presenter
{
private:
	ptr<GlDevice> device;
	ptr<GlFrameBuffer> frameBuffer;
	HDC hdc;
	ptr<Platform::Win32Window> window;
	int width, height;
	int currentSwapInterval, targetSwapInterval;

public:
	WglPresenter(ptr<GlDevice> device, ptr<GlFrameBuffer> frameBuffer, HDC hdc, ptr<Platform::Win32Window> window);
	~WglPresenter();

	void Bind(HGLRC hglrc);

	// Presenter's methods.
	ptr<Device> GetDevice() const;
	int GetWidth() const;
	int GetHeight() const;
	ptr<FrameBuffer> GetFrameBuffer() const;
	void SetMode(ptr<MonitorMode> mode);
	void SetSwapInterval(int swapInterval);
	void Present();
	void Resize(int width, int height);
};

END_INANITY_GRAPHICS

#endif
