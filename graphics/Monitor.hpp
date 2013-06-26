#ifndef ___INANITY_GRAPHICS_MONITOR_HPP___
#define ___INANITY_GRAPHICS_MONITOR_HPP___

#include "graphics.hpp"
#include "../platform/platform.hpp"
#include "../String.hpp"
#include <vector>
#ifdef ___INANITY_WINDOWS
#include "../windows.hpp"
#endif

BEGIN_INANITY_PLATFORM

class Window;

END_INANITY_PLATFORM

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
	/// Создать свой режим.
	/** Монитор может подкорректировать его, чтобы он был поддерживаемым.
	Может быть возвращён 0, если никак нельзя создать поддерживаемый режим. */
	virtual ptr<MonitorMode> TryCreateMode(int width, int height) = 0;

	/// Создать окно по центру монитора.
	virtual ptr<Platform::Window> CreateWindowCentered(const String& title, int width, int height) = 0;

#ifdef ___INANITY_WINDOWS
	/// Получить координаты монитора на рабочем столе.
	virtual RECT GetRect() const = 0;
#endif
};

END_INANITY_GRAPHICS

#endif
