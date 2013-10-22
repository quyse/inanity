#ifndef ___INANITY_PLATFORM_X11_WINDOW_HPP___
#define ___INANITY_PLATFORM_X11_WINDOW_HPP___

#include "Window.hpp"
#include "x11.hpp"
#include "../Handler.hpp"
#include "../input/input.hpp"

#ifndef ___INANITY_LINUX
#error X11Window implemented only on Linux.
#endif

BEGIN_INANITY_GRAPHICS

class X11Output;

END_INANITY_GRAPHICS

BEGIN_INANITY_INPUT

class X11Manager;

END_INANITY_INPUT

BEGIN_INANITY_PLATFORM

class X11Display;

class X11Window : public Window
{
private:
	ptr<X11Display> display;
	xcb_window_t handle;
	xcb_colormap_t colormap;

	/// Graphics output to receive notifications about resize.
	Graphics::X11Output* output;
	/// Input manager to receive input notifications.
	ptr<Input::X11Manager> inputManager;

private:
	X11Window(ptr<X11Display> display, xcb_window_t handle, xcb_colormap_t colormap);
	~X11Window();

	bool Do(Handler* activeHandler);

	void UpdateMouseLock();
	void UpdateCursorVisible();

public:
	// методы Window
	void SetTitle(const String& title);
	void Close();
	void Run(ptr<Handler> activeHandler);
	ptr<Graphics::Output> CreateOutput();

	ptr<X11Display> GetDisplay() const;
	xcb_window_t GetHandle() const;
	void SetOutput(Graphics::X11Output* output);
	void SetInputManager(ptr<Input::X11Manager> inputManager);

	/// Создать окно для OpenGL.
	static ptr<X11Window> CreateForOpenGL(ptr<X11Display> display, int screenNumber, int left, int top, int width, int height);
};

END_INANITY_PLATFORM

#endif
