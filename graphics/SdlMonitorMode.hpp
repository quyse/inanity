#ifndef ___INANITY_GRAPHICS_SDL_MONITOR_MODE_HPP___
#define ___INANITY_GRAPHICS_SDL_MONITOR_MODE_HPP___

#include "MonitorMode.hpp"
#include <SDL2/SDL_video.h>

BEGIN_INANITY_GRAPHICS

class SdlMonitor;

/// SDL monitor mode.
class SdlMonitorMode : public MonitorMode
{
private:
	SDL_DisplayMode mode;
	String name;

public:
	SdlMonitorMode(const SDL_DisplayMode& mode);

	//*** MonitorMode's methods.
	int GetWidth() const;
	int GetHeight() const;
	float GetRefreshRate() const;
	String GetName() const;
};

END_INANITY_GRAPHICS

#endif
