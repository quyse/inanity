#ifndef ___INANITY_GRAPHICS_EMS_MONITOR_HPP___
#define ___INANITY_GRAPHICS_EMS_MONITOR_HPP___

#include "Monitor.hpp"

#ifndef ___INANITY_PLATFORM_EMSCRIPTEN
#error EmsMonitor is for Emscripten platform
#endif

BEGIN_INANITY_GRAPHICS

/// Dummy monitor class for Emscripten graphics.
class EmsMonitor : public Monitor
{
private:
	std::vector<ptr<MonitorMode> > modes;

public:
	EmsMonitor();

	//*** Monitor's methods.
	String GetId() const;
	String GetName() const;
	const std::vector<ptr<MonitorMode> >& GetModes();
	ptr<MonitorMode> TryCreateMode(int width, int height);
	ptr<Platform::Window> CreateDefaultWindow(const String& title, int width, int height);
};

END_INANITY_GRAPHICS

#endif
