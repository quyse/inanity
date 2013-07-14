#ifndef ___INANITY_GRAPHICS_GL_PRESENTER_HPP___
#define ___INANITY_GRAPHICS_GL_PRESENTER_HPP___

#include "Presenter.hpp"

#ifdef ___INANITY_WINDOWS
#include "../windows.hpp"
#endif

BEGIN_INANITY_GRAPHICS

class GlDevice;
class GlRenderBuffer;
class X11Output;

/// Класс Presenter для OpenGL.
class GlPresenter : public Presenter
{
private:
	/// Устройство.
	ptr<GlDevice> device;

#ifdef ___INANITY_WINDOWS
	/// Контекст окна.
	HDC hdc;
#endif

#ifdef ___INANITY_LINUX
	ptr<X11Output> output;
#endif

	/// Вторичный буфер.
	ptr<GlRenderBuffer> backBuffer;

public:
#ifdef ___INANITY_WINDOWS
	GlPresenter(ptr<GlDevice> device, HDC hdc, ptr<GlRenderBuffer> backBuffer);
#endif
#ifdef ___INANITY_LINUX
	GlPresenter(ptr<GlDevice> device, ptr<X11Output> output, ptr<GlRenderBuffer> backBuffer);
	~GlPresenter();
#endif

	// методы Presenter
	ptr<Device> GetDevice();
	ptr<RenderBuffer> GetBackBuffer();
	void SetMode(ptr<MonitorMode> mode);
	void Present();
	void Resize(int width, int height);
};

END_INANITY_GRAPHICS

#endif
