#ifndef ___INANITY_GRAPHICS_X11_MONITOR_MODE_HPP___
#define ___INANITY_GRAPHICS_X11_MONITOR_MODE_HPP___

#include "MonitorMode.hpp"

BEGIN_INANITY_GRAPHICS

/// X11 monitor mode.
class X11MonitorMode : public MonitorMode
{
private:


public:
	X11MonitorMode();

	//*** MonitorMode's methods.
	int GetWidth() const;
	int GetHeight() const;
	float GetRefreshRate() const;
	String GetName() const;
};

END_INANITY_GRAPHICS

#endif
