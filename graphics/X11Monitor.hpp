#ifndef ___INANITY_GRAPHICS_X11_MONITOR_HPP___
#define ___INANITY_GRAPHICS_X11_MONITOR_HPP___

#include "Monitor.hpp"
#include "../platform/x11.hpp"

BEGIN_INANITY_PLATFORM

class X11Display;

END_INANITY_PLATFORM

BEGIN_INANITY_GRAPHICS

/// X11 screen as a Monitor.
class X11Monitor : public Monitor
{
private:
	ptr<Platform::X11Display> display;
	int screenNumber;
	::Screen* screen;
	String id, name;
	std::vector<ptr<MonitorMode> > modes;
	bool modesInitialized;

public:
	X11Monitor(ptr<Platform::X11Display> display, const String& displayName, int screenNumber);
	~X11Monitor();

	// Monitor's methods.
	String GetId() const;
	String GetName() const;
	const std::vector<ptr<MonitorMode> >& GetModes();
	ptr<MonitorMode> TryCreateMode(int width, int height);
	ptr<Platform::Window> CreateDefaultWindow(const String& title, int width, int height);
};

END_INANITY_GRAPHICS

#endif
