#ifndef ___INANITY_GRAPHICS_GL_PRESENTER_HPP___
#define ___INANITY_GRAPHICS_GL_PRESENTER_HPP___

#include "Presenter.hpp"

#ifdef ___INANITY_WINDOWS
#include "../windows.hpp"
#endif

BEGIN_INANITY_GRAPHICS

class GlDevice;
class GlRenderBuffer;

/// Класс Presenter для OpenGL.
class GlPresenter : public Presenter
{
private:
	/// Устройство.
	ptr<GlDevice> device;

#ifdef ___INANITY_WINDOWS
	/// Контекст окна.
	HDC hdc;
	/// 
#endif

	/// Вторичный буфер.
	ptr<GlRenderBuffer> backBuffer;

public:
#ifdef ___INANITY_WINDOWS
	GlPresenter(ptr<GlDevice> device, HDC hdc, ptr<GlRenderBuffer> backBuffer);
#endif

	// методы Presenter
	ptr<Device> GetDevice();
	ptr<RenderBuffer> GetBackBuffer();
	void SetMode(const PresentMode& mode);
	void Present();
};

END_INANITY_GRAPHICS

#endif
