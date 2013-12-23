#ifndef ___INANITY_GRAPHICS_SDL_MONITOR_HPP___
#define ___INANITY_GRAPHICS_SDL_MONITOR_HPP___

#include "Monitor.hpp"
#include "../platform/platform.hpp"

BEGIN_INANITY_PLATFORM

class Sdl;

END_INANITY_PLATFORM

BEGIN_INANITY_GRAPHICS

class SdlAdapter;

/// SDL monitor.
class SdlMonitor : public Monitor
{
private:
	ptr<Platform::Sdl> sdl;
	int displayIndex;
	String name;
	MonitorModes modes;
	bool modesInitialized;

public:
	SdlMonitor(int displayIndex);

	//*** Monitor's methods.
	String GetId() const;
	String GetName() const;
	const MonitorModes& GetModes();
	ptr<MonitorMode> TryCreateMode(int width, int height);
	ptr<Platform::Window> CreateDefaultWindow(const String& title, int width, int height);
};

END_INANITY_GRAPHICS

#endif
