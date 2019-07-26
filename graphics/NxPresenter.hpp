#ifndef ___INANITY_GRAPHICS_NX_PRESENTER_HPP___
#define ___INANITY_GRAPHICS_NX_PRESENTER_HPP___

#include "GlPresenter.hpp"
#include "../platform/platform.hpp"

BEGIN_INANITY_PLATFORM

class NxWindow;

END_INANITY_PLATFORM

BEGIN_INANITY_GRAPHICS

/// Presenter class for Nintendo Switch graphics.
class NxPresenter : public GlPresenter
{
private:
	ptr<Platform::NxWindow> window;

public:
	NxPresenter(ptr<GlDevice> device, ptr<GlFrameBuffer> frameBuffer, ptr<Platform::NxWindow> window);

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
