#ifndef ___INANITY_GRAPHICS_SDL_PRESENTER_HPP___
#define ___INANITY_GRAPHICS_SDL_PRESENTER_HPP___

#include "GlPresenter.hpp"
#include "../platform/SdlWindow.hpp"

BEGIN_INANITY_GRAPHICS

/// Presenter class using SDL.
class SdlPresenter : public GlPresenter
{
private:
	ptr<Platform::SdlWindow> window;
	/// Size of back buffer.
	int width, height;
	int currentSwapInterval, targetSwapInterval;

public:
	SdlPresenter(ptr<GlDevice> device, ptr<GlFrameBuffer> frameBuffer, ptr<Platform::SdlWindow> window);
	~SdlPresenter();

	void Bind(SDL_GLContext sdlContext);

	//*** Presenter's methods.
	int GetWidth() const;
	int GetHeight() const;
	void SetMode(ptr<MonitorMode> mode);
	void SetSwapInterval(int swapInterval);
	void Present();
	void Resize(int width, int height);
};

END_INANITY_GRAPHICS

#endif
