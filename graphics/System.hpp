#ifndef ___INANITY_GRAPHICS_SYSTEM_HPP___
#define ___INANITY_GRAPHICS_SYSTEM_HPP___

#include "graphics.hpp"
#include <vector>

BEGIN_INANITY_GRAPHICS

class Adapter;
class Device;
class Context;

/// Класс графической подсистемы.
/** Абстрактная фабрика для создания объектов
графических устройств. */
class System : public Object
{
public:
	/// Получить графические адаптеры.
	virtual const std::vector<ptr<Adapter> >& GetAdapters() = 0;

	/// Создать главное графическое устройство.
	virtual ptr<Device> CreateDevice(ptr<Adapter> adapter) = 0;
	/// Создать основной контект для графического устройства.
	virtual ptr<Context> CreateContext(ptr<Device> device) = 0;
};

END_INANITY_GRAPHICS

#endif
