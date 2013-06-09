#ifndef ___INANITY_GRAPHICS_WIN32_MONITOR_MODE_HPP___
#define ___INANITY_GRAPHICS_WIN32_MONITOR_MODE_HPP___

#include "MonitorMode.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс режима экрана Win32.
class Win32MonitorMode : public MonitorMode
{
private:
	DEVMODE info;
	String name;

public:
	Win32MonitorMode(const DEVMODE& info);

	//*** методы MonitorMode.
	int GetWidth() const;
	int GetHeight() const;
	float GetRefreshRate() const;
	String GetName() const;

	const DEVMODE& GetInfo() const;
};

END_INANITY_GRAPHICS

#endif
