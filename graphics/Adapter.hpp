#ifndef ___INANITY_GRAPHICS_ADAPTER_HPP___
#define ___INANITY_GRAPHICS_ADAPTER_HPP___

#include "Monitor.hpp"
#include "../String.hpp"
#include <vector>

BEGIN_INANITY_GRAPHICS

class Monitor;

/// Структура с информацией о графическом адаптере.
/** Только id может использоваться в качестве идентификатора.
Остальные строковые поля для красоты, то есть для пользователя. */
class Adapter : public Object
{
public:
	typedef std::vector<ptr<Adapter> > Adapters;
	typedef std::vector<ptr<Monitor> > Monitors;

public:
	/// Получить уникальный идентификатор адаптера.
	virtual String GetId() const = 0;
	/// Человекопонятное имя адаптера.
	virtual String GetName() const = 0;
	/// Получить мониторы.
	virtual const Monitors& GetMonitors() = 0;
};

END_INANITY_GRAPHICS

#endif
