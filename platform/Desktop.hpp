#ifndef ___INANITY_PLATFORM_DESKTOP_HPP___
#define ___INANITY_PLATFORM_DESKTOP_HPP___

#include "platform.hpp"
#include "../graphics/graphics.hpp"
#include "../String.hpp"

BEGIN_INANITY_GRAPHICS

class Monitor;

END_INANITY_GRAPHICS

BEGIN_INANITY_PLATFORM

class Window;

class Desktop
{
public:
	/// Создать окно в нужном месте (в координатах рабочего стола).
	static ptr<Window> CreateWindowAt(const String& title, int left, int top, int width, int height);
	/// Создать окно по центру заданного монитора.
	static ptr<Window> CreateWindowCentered(const String& title, ptr<Graphics::Monitor> monitor, int width, int height);
};

END_INANITY_PLATFORM

#endif
