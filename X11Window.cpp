#include "X11Window.hpp"
#include "X11Display.hpp"
#include "graphics/X11Output.hpp"
#include "graphics/opengl.hpp"
#include "Exception.hpp"

BEGIN_INANITY

X11Window::X11Window(ptr<X11Display> display, Handle handle) : handle(handle) {}

X11Window::~X11Window()
{
	XFree(visualInfo);
	Close();
}

void X11Window::SetTitle(const String& title)
{
	char* stringList = (char*)title.c_str();
	XTextProperty textProperty;
	if(XStringListToTextProperty(&stringList, 1, &textProperty))
	{
		XSetWMName(display->GetDisplay(), handle, &textProperty);
		XFree(textProperty.value);
	}
}

void X11Window::Close()
{
	if(display && handle)
	{
		XDestroyWindow(display->GetDisplay(), handle);
		display = 0;
		handle = 0;
	}
}

ptr<Graphics::Output> X11Window::CreateOutput()
{
	return NEW(Graphics::X11Output(this));
}

ptr<X11Display> X11Window::GetDisplay() const
{
	return display;
}

X11Window::Handle X11Window::GetHandle() const
{
	return handle;
}

XVisualInfo* X11Window::GetXVisualInfo() const
{
	return visualInfo;
}

ptr<X11Window> X11Window::CreateForOpenGL(ptr<X11Display> display)
{
	try
	{
		Display* d = display->GetDisplay();

		// получить, поддерживает ли дисплей расширение GLX
		if(!glXQueryExtension(d, NULL, NULL))
			THROW_PRIMARY_EXCEPTION("Display doesn't support GLX extension");

		// выбрать нужный visual в дисплее
		static int visualAttrs[] =
		{
			GLX_RGBA, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 24, None
		};
		XVisualInfo* visualInfo = glXChooseVisual(d, DefaultScreen(d), visualAttrs);
		if(!visualInfo)
			THROW_PRIMARY_EXCEPTION("Can't choose X11 visual");

		// создать окно
		XSetWindowAttributes windowAttrs;
		windowAttrs.event_mask
			= ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask;
		Handle handle = XCreateWindow(
			d, // display
			RootWindow(d, DefaultScreen(d)), // parent window
			0, 0, // x, y
			1, 1, // width and height
			0, // border width
			visualInfo->depth, // depth
			InputOutput, // window type
			visualInfo->visual, // visual
			CWBorderPixel | CWEventMask, // mask of attributes set
			&windowAttrs // attributes
		);
		if(!handle)
		{
			XFree(visualInfo);
			THROW_PRIMARY_EXCEPTION("Can't create window");
		}

		return NEW(X11Window(display, handle, visualInfo));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create X11 window for OpenGL", exception);
	}
}

END_INANITY
