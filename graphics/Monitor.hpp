#ifndef ___INANITY_GRAPHICS_MONITOR_HPP___
#define ___INANITY_GRAPHICS_MONITOR_HPP___

#include "graphics.hpp"
#include "../String.hpp"
#include <vector>

BEGIN_INANITY_GRAPHICS

class MonitorMode;

/// Абстрактный класс монитора, подключенного к адаптеру.
class Monitor : public Object
{
public:
	/// Получить идентификатор монитора.
	virtual String GetId() const = 0;
	/// Получить имя монитора.
	virtual String GetName() const = 0;
	/// Получить режимы, поддерживаемые монитором.
	virtual const std::vector<ptr<MonitorMode> >& GetModes() = 0;
};

END_INANITY_GRAPHICS

#endif
