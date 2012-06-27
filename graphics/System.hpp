#ifndef ___INANITY_GRAPHICS_SYSTEM_HPP___
#define ___INANITY_GRAPHICS_SYSTEM_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

class Device;

/// Класс графической подсистемы.
/** Абстрактная фабрика для создания объектов
графических устройств. */
class System : public Object
{
public:
	/// Информация о графическом устройстве.
	class DeviceInfo
	{
	};

public:
	/// Создать главное графическое устройство.
	virtual ptr<Device> CreatePrimaryDevice() = 0;
};

END_INANITY_GRAPHICS

#endif
