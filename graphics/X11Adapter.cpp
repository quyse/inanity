#include "X11Adapter.hpp"
#include "X11Monitor.hpp"
#include "../platform/X11Display.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

X11Adapter::X11Adapter(const String& displayName, ptr<Platform::X11Display> display)
: displayName(displayName), display(display), monitorsInitialized(false) {}

String X11Adapter::GetId() const
{
	return displayName;
}

String X11Adapter::GetName() const
{
	return displayName;
}

const std::vector<ptr<Monitor> >& X11Adapter::GetMonitors()
{
	BEGIN_TRY();

	if(!monitorsInitialized)
	{
		int monitorsCount = XScreenCount(display->GetDisplay());
		for(int i = 0; i < monitorsCount; ++i)
			monitors.push_back(NEW(X11Monitor(this, i)));
		monitorsInitialized = true;
	}

	return monitors;

	END_TRY("Can't get monitors");
}

ptr<Platform::X11Display> X11Adapter::GetDisplay() const
{
	return display;
}

void X11Adapter::GetAdapters(std::vector<ptr<Adapter> >& adapters)
{
	BEGIN_TRY();

	// for now display specified in DISPLAY environment variable is only returned
	const char* displayName = getenv("DISPLAY");
	if(!displayName)
		THROW_PRIMARY_EXCEPTION("DISPLAY variable is not set");
	adapters.push_back(NEW(X11Adapter(displayName, Platform::X11Display::Open(displayName))));

	END_TRY("Can't get adapters");
}

END_INANITY_GRAPHICS
