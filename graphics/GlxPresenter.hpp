#ifndef ___INANITY_GRAPHICS_GLX_PRESENTER_HPP___
#define ___INANITY_GRAPHICS_GLX_PRESENTER_HPP___

#include "Presenter.hpp"
#include "opengl.hpp"

BEGIN_INANITY_GRAPHICS

class GlDevice;
class GlFrameBuffer;
class X11Output;

/// Presenter class for OpenGL on X.org.
class GlxPresenter : public Presenter
{
private:
	ptr<GlDevice> device;
	ptr<GlFrameBuffer> frameBuffer;
	ptr<X11Output> output;
	GLXWindow glxWindow;

public:
	GlxPresenter(ptr<GlDevice> device, ptr<GlFrameBuffer> frameBuffer, ptr<X11Output> output, GLXWindow glxWindow);
	~GlxPresenter();

	// Presenter's methods.
	ptr<Device> GetDevice();
	ptr<FrameBuffer> GetFrameBuffer();
	void SetMode(ptr<MonitorMode> mode);
	void Present();
	void Resize(int width, int height);
};

END_INANITY_GRAPHICS

#endif
