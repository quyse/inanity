#ifndef ___INANITY_PLATFORM_NPAPI_PRESENTER_HPP___
#define ___INANITY_PLATFORM_NPAPI_PRESENTER_HPP___

#include "platform.hpp"
#include "../graphics/Presenter.hpp"

#if defined(___INANITY_PLATFORM_WINDOWS)
#include "windows.hpp"
#elif defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD)
#include <X11/Xlib.h>
#else
#error Unknown platform
#endif

BEGIN_INANITY

class MemoryFile;

END_INANITY

BEGIN_INANITY_GRAPHICS

class Device;
class RenderBuffer;
class FrameBuffer;

END_INANITY_GRAPHICS

BEGIN_INANITY_PLATFORM

/// Cross-platform graphics presenter for NPAPI plugin.
class NpapiPresenter : public Graphics::Presenter
{
private:
	ptr<Graphics::Device> device;
	/// Renderbuffer to draw into.
	mutable ptr<Graphics::RenderBuffer> renderBuffer;
	/// Actual size of render buffer.
	mutable int renderBufferWidth;
	mutable int renderBufferHeight;
	/// Framebuffer containing renderbuffer.
	mutable ptr<Graphics::FrameBuffer> frameBuffer;
	/// Size of present area.
	int width;
	int height;

#if defined(___INANITY_PLATFORM_WINDOWS)

	mutable BITMAPINFOHEADER bfh;
	mutable ptr<MemoryFile> imageFile;

#elif defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD)

	mutable XImage image;
	mutable ptr<MemoryFile> imageFile;

#endif

public:
	NpapiPresenter(ptr<Graphics::Device> device);
	~NpapiPresenter();

	//*** Presenter's methods.
	ptr<Graphics::Device> GetDevice() const;
	int GetWidth() const;
	int GetHeight() const;
	ptr<Graphics::FrameBuffer> GetFrameBuffer() const;
	void SetMode(ptr<Graphics::MonitorMode> mode);
	void Present();
	void Resize(int width, int height);

#ifdef ___INANITY_PLATFORM_WINDOWS
	void PresentOnHdc(HDC hdc, int left, int top);
#endif
#if defined(___INANITY_PLATFORM_LINUX) || defined(___INANITY_PLATFORM_FREEBSD)
	void PresentOnXGraphicsExposeEvent(const XGraphicsExposeEvent& e, int left, int top);
#endif
};

END_INANITY_PLATFORM

#endif
