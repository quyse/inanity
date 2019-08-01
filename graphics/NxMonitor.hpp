#ifndef ___INANITY_GRAPHICS_NX_MONITOR_HPP___
#define ___INANITY_GRAPHICS_NX_MONITOR_HPP___

#include "Monitor.hpp"
#include <nn/vi.h>

BEGIN_INANITY_GRAPHICS

/// Nintendo Switch monitor.
class NxMonitor : public Monitor
{
private:
	nn::vi::DisplayInfo displayInfo;
	static const MonitorModes modes;

public:
	NxMonitor(const nn::vi::DisplayInfo& displayInfo);
	~NxMonitor();

	// Monitor's methods.
	String GetId() const;
	String GetName() const;
	const MonitorModes& GetModes();
	ptr<MonitorMode> TryCreateMode(int width, int height);
	ptr<Platform::Window> CreateDefaultWindow(const String& title, int width, int height);
};

END_INANITY_GRAPHICS

#endif
