#ifndef ___INANITY_GRAPHICS_GLX_PRESENTER_HPP___
#define ___INANITY_GRAPHICS_GLX_PRESENTER_HPP___

#include "Presenter.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

class GlDevice;
class GlRenderBuffer;
class X11Output;

/// Presenter class for OpenGL on X.org.
class GlxPresenter : public Presenter
{
private:
	ptr<GlDevice> device;
	ptr<GlRenderBuffer> backBuffer;
	ptr<X11Output> output;
	GLXWindow glxWindow;

public:
	GlxPresenter(ptr<GlDevice> device, ptr<GlRenderBuffer> backBuffer, ptr<X11Output> output, GLXWindow glxWindow);
	~GlxPresenter();

	// Presenter's methods.
	ptr<Device> GetDevice();
	ptr<RenderBuffer> GetBackBuffer();
	void SetMode(ptr<MonitorMode> mode);
	void Present();
	void Resize(int width, int height);
};

END_INANITY_GRAPHICS

#endif
