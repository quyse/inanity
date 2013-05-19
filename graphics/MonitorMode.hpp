#ifndef ___INANITY_GRAPHICS_MONITOR_MODE_HPP___
#define ___INANITY_GRAPHICS_MONITOR_MODE_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

/// Абстрактный класс поддерживаемого режима монитора.
class MonitorMode : public Object
{
public:
	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
	virtual float GetRefreshRate() const = 0;
};

END_INANITY_GRAPHICS

#endif
