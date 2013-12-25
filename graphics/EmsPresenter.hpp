#ifndef ___INANITY_GRAPHICS_EMS_PRESENTER_HPP___
#define ___INANITY_GRAPHICS_EMS_PRESENTER_HPP___

#include "Presenter.hpp"
#include "../platform/platform.hpp"

BEGIN_INANITY_PLATFORM

class EmsWindow;

END_INANITY_PLATFORM

BEGIN_INANITY_GRAPHICS

class GlDevice;
class GlFrameBuffer;

/// Presenter class for Emscripten graphics.
class EmsPresenter : public Presenter
{
private:
	ptr<GlDevice> device;
	ptr<GlFrameBuffer> frameBuffer;
	ptr<Platform::EmsWindow> window;
	int width, height;

public:
	EmsPresenter(ptr<GlDevice> device, ptr<GlFrameBuffer> frameBuffer, ptr<Platform::EmsWindow> window);
	~EmsPresenter();

	//*** Presenter's methods.
	ptr<Device> GetDevice() const;
	int GetWidth() const;
	int GetHeight() const;
	ptr<FrameBuffer> GetFrameBuffer() const;
	void SetMode(ptr<MonitorMode> mode);
	void Present();
	void Resize(int width, int height);
};

END_INANITY_GRAPHICS

#endif
