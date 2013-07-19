#ifndef ___INANITY_GRAPHICS_WGL_PRESENTER_HPP___
#define ___INANITY_GRAPHICS_WGL_PRESENTER_HPP___

#include "Presenter.hpp"
#include "../windows.hpp"

BEGIN_INANITY_GRAPHICS

class GlDevice;
class GlRenderBuffer;

/// Presenter class for OpenGL on Windows.
class WglPresenter : public Presenter
{
private:
	ptr<GlDevice> device;
	ptr<GlRenderBuffer> backBuffer;
	HDC hdc;

public:
	WglPresenter(ptr<GlDevice> device, ptr<GlRenderBuffer> backBuffer, HDC hdc);

	// Presenter's methods.
	ptr<Device> GetDevice();
	ptr<RenderBuffer> GetBackBuffer();
	void SetMode(ptr<MonitorMode> mode);
	void Present();
	void Resize(int width, int height);
};

END_INANITY_GRAPHICS

#endif
