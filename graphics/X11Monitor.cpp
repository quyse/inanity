#include "X11Monitor.hpp"
#include "X11MonitorMode.hpp"
#include "../platform/X11Display.hpp"
#include "../platform/X11Window.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

X11Monitor::X11Monitor(ptr<Platform::X11Display> display, const String& displayName, int screenNumber)
: display(display), screenNumber(screenNumber), screen(screen), modesInitialized(false)
{
	BEGIN_TRY();

	std::ostringstream s;
	s << displayName << ':' << screenNumber;
	id = s.str();

	screen = XScreenOfDisplay(display->GetDisplay(), screenNumber);
	if(!screen)
		THROW_PRIMARY_EXCEPTION("Can't get screen");

	END_TRY("Can't create X11 monitor");
}

X11Monitor::~X11Monitor()
{
	if(screen)
		XFree(screen);
}

String X11Monitor::GetId() const
{
	return id;
}

String X11Monitor::GetName() const
{
	return id;
}

const std::vector<ptr<MonitorMode> >& X11Monitor::GetModes()
{
	if(!modesInitialized)
	{
		modesInitialized = true;
	}

	return modes;
}

ptr<MonitorMode> X11Monitor::TryCreateMode(int width, int height)
{
	return 0;
}

ptr<Platform::Window> X11Monitor::CreateDefaultWindow(const String& title, int width, int height)
{
	BEGIN_TRY();

	ptr<Platform::X11Window> window = Platform::X11Window::CreateForOpenGL(display, screenNumber, 0, 0, width, height);
	window->SetTitle(title);

	return window;

	END_TRY("Can't create default window on X11 monitor");
}

END_INANITY_GRAPHICS
