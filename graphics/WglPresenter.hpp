#ifndef ___INANITY_GRAPHICS_WGL_PRESENTER_HPP___
#define ___INANITY_GRAPHICS_WGL_PRESENTER_HPP___

#include "Presenter.hpp"
#include "../windows.hpp"

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

public:
	WglPresenter(ptr<GlDevice> device, ptr<GlFrameBuffer> frameBuffer, HDC hdc);

	// Presenter's methods.
	ptr<Device> GetDevice() const;
	ptr<FrameBuffer> GetFrameBuffer() const;
	void SetMode(ptr<MonitorMode> mode);
	void Present();
	void Resize(int width, int height);
};

END_INANITY_GRAPHICS

#endif
