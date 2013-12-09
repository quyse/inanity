#include "X11Window.hpp"
#include "X11Display.hpp"
#include "../graphics/X11Output.hpp"
#include "../graphics/opengl.hpp"
#include "../input/X11Manager.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_PLATFORM

X11Window::X11Window(ptr<X11Display> display, xcb_window_t handle, xcb_colormap_t colormap)
: display(display), handle(handle), colormap(colormap), output(0) {}

X11Window::~X11Window()
{
	Close();
}

void X11Window::SetTitle(const String& title)
{
	xcb_change_property(
		display->GetConnection(),
		XCB_PROP_MODE_REPLACE,
		handle,
		XCB_ATOM_WM_NAME,
		XCB_ATOM_STRING,
		8,
		title.length(),
		title.c_str()
	);
}

void X11Window::Close()
{
	if(display && handle)
	{
		xcb_destroy_window(display->GetConnection(), handle);
		handle = 0;
		if(colormap)
		{
			xcb_free_colormap(display->GetConnection(), colormap);
			colormap = 0;
		}
		inputManager = 0;
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

void X11Window::PlaceCursor(int x, int y)
{
	// not implemented yet
}

ptr<X11Display> X11Window::GetDisplay() const
{
	return display;
}

xcb_window_t X11Window::GetHandle() const
{
	return handle;
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
	xcb_connection_t* connection = display->GetConnection();

	/* Find XCB screen */
	xcb_screen_iterator_t i;
	int j;
	for(
		i = xcb_setup_roots_iterator(xcb_get_setup(connection)), j = 0;
		i.rem && j < screenNumber;
		xcb_screen_next(&i), ++j);
	xcb_screen_t* screen = i.data;

	// получить, поддерживает ли дисплей расширение GLX
	if(!glXQueryExtension(d, NULL, NULL))
		THROW("Display doesn't support GLX extension");

	// создать colormap
	xcb_colormap_t colormap = xcb_generate_id(connection);
	xcb_create_colormap(connection, XCB_COLORMAP_ALLOC_NONE, colormap, screen->root, screen->root_visual);

	// создать окно
	xcb_window_t window = xcb_generate_id(connection);

	uint32_t values[] = {
		XCB_EVENT_MASK_STRUCTURE_NOTIFY |
		XCB_EVENT_MASK_EXPOSURE |
		XCB_EVENT_MASK_ENTER_WINDOW |
		XCB_EVENT_MASK_LEAVE_WINDOW |
		XCB_EVENT_MASK_KEY_PRESS |
		XCB_EVENT_MASK_KEY_RELEASE |
		XCB_EVENT_MASK_BUTTON_PRESS |
		XCB_EVENT_MASK_BUTTON_RELEASE |
		XCB_EVENT_MASK_POINTER_MOTION,
		colormap,
		0
	};

	xcb_create_window(
		connection,
		XCB_COPY_FROM_PARENT,
		window,
		screen->root,
		left, top, width, height,
		0, // border width,
		XCB_WINDOW_CLASS_INPUT_OUTPUT,
		XCB_COPY_FROM_PARENT,
		XCB_CW_EVENT_MASK |
		XCB_CW_COLORMAP,
		values
	);

	xcb_map_window(connection, window);

	return NEW(X11Window(display, window, colormap));

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
				handle = 0;
				inputManager = 0;
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

void X11Window::UpdateMouseLock()
{
	// not implemented yet
}

void X11Window::UpdateCursorVisible()
{
	// not implemented yet
}

END_INANITY_PLATFORM
