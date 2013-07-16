#include "X11Window.hpp"
#include "X11Display.hpp"
#include "../graphics/X11Output.hpp"
#include "../graphics/opengl.hpp"
#include "../input/X11Manager.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_PLATFORM

X11Window::X11Window(ptr<X11Display> display, Handle handle, XVisualInfo* visualInfo)
: display(display), handle(handle), visualInfo(visualInfo), output(0) {}

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
		Handle h = handle;
		handle = 0;
		inputManager = 0;
		XDestroyWindow(display->GetDisplay(), h);
	}
}

void X11Window::Run(ptr<Handler> activeHandler)
{
	while(Do(activeHandler));
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

XVisualInfo* X11Window::GetVisualInfo() const
{
	return visualInfo;
}

void X11Window::SetOutput(Graphics::X11Output* output)
{
	this->output = output;
}

void X11Window::SetInputManager(ptr<Input::X11Manager> inputManager)
{
	this->inputManager = inputManager;
}

ptr<X11Window> X11Window::CreateForOpenGL(ptr<X11Display> display, int screenNumber, int left, int top, int width, int height)
{
	BEGIN_TRY();

	::Display* d = display->GetDisplay();

	// получить, поддерживает ли дисплей расширение GLX
	if(!glXQueryExtension(d, NULL, NULL))
		THROW_PRIMARY_EXCEPTION("Display doesn't support GLX extension");

	// выбрать нужный visual в дисплее
	static int visualAttrs[] =
	{
		GLX_RGBA, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 24, None
	};
	XVisualInfo* visualInfo = glXChooseVisual(d, screenNumber, visualAttrs);
	if(!visualInfo)
		THROW_PRIMARY_EXCEPTION("Can't choose X11 visual");

	// создать окно
	XSetWindowAttributes windowAttrs;
	windowAttrs.event_mask =
		StructureNotifyMask | // ConfigureNotify, DestroyNotify
		ExposureMask |
		EnterWindowMask | LeaveWindowMask |
		KeyPressMask |
		KeyReleaseMask |
		ButtonPressMask |
		ButtonReleaseMask |
		PointerMotionMask;
	Handle handle = XCreateWindow(
		d, // display
		RootWindow(d, screenNumber), // parent window
		left, top, // x, y
		width, height, // width and height
		0, // border width
		CopyFromParent,//visualInfo->depth, // depth
		InputOutput, // window type
		CopyFromParent,//visualInfo->visual, // visual
		CWBorderPixel | CWEventMask, // mask of attributes set
		&windowAttrs // attributes
	);
	if(!handle)
	{
		XFree(visualInfo);
		THROW_PRIMARY_EXCEPTION("Can't create window");
	}

	return NEW(X11Window(display, handle, visualInfo));

	END_TRY("Can't create X11 window for OpenGL");
}

bool X11Window::Do(Handler* activeHandler)
{
	::Display* d = display->GetDisplay();
	// cycle for message-consume cycles
	// message-consume cycles are there to reduce number of useless XPending calls
	bool active = true;
	do
	{
		XEvent event;
		// get a number of events in queue (with flushing as nesessary)
		int eventsCount = XPending(d);
		// cycle for all events in queue (and after if window in not active)
		for(int i = 0; !active || i < eventsCount; ++i)
		{
			// if there are no events, this call will wait for the next
			XNextEvent(d, &event);

			// give input manager a chance to process the event
			if(inputManager)
				inputManager->Process(event);

			switch(event.type)
			{
			case ConfigureNotify:
				// notify output about resize
				if(output)
					output->Resize(event.xconfigure.width, event.xconfigure.height);
				break;
			case DestroyNotify:
				// exit
				return false;
			}
		}
	}
	while(!active);

	// if there are no events now, do the work
	if(active)
	{
		if(inputManager)
			inputManager->Update();
		activeHandler->Fire();
	}

	return true;
}

END_INANITY_PLATFORM
