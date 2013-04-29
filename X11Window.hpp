#ifndef ___INANITY_X11_WINDOW_HPP___
#define ___INANITY_X11_WINDOW_HPP___

#include "Window.hpp"
#include "x11.hpp"

#ifndef ___INANITY_LINUX
#error X11Window implemented only on Linux.
#endif

BEGIN_INANITY

class X11Display;

class X11Window : public Window
{
private:
	typedef ::Window Handle;

	/// Дисплей окна.
	ptr<X11Display> display;
	/// Хендл окна.
	Handle handle;
	XVisualInfo* visualInfo;

private:
	X11Window(ptr<X11Display> display, Handle handle, XVisualInfo* visualInfo);
	~X11Window();

public:
	// методы Window
	void SetTitle(const String& title);
	void Close();
	ptr<Graphics::Output> CreateOutput();

	ptr<X11Display> GetDisplay() const;
	Handle GetHandle() const;
	XVisualInfo* GetXVisualInfo() const;

	/// Создать окно для OpenGL.
	static ptr<X11Window> CreateForOpenGL(ptr<X11Display> display);
};

END_INANITY

#endif
