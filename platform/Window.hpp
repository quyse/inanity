#ifndef ___INANITY_PLATFORM_WINDOW_HPP___
#define ___INANITY_PLATFORM_WINDOW_HPP___

#include "platform.hpp"
#include "../String.hpp"
#include "../graphics/graphics.hpp"

BEGIN_INANITY_GRAPHICS

class Output;

END_INANITY_GRAPHICS

BEGIN_INANITY_PLATFORM

/// Абстрактный класс окна.
class Window : public Object
{
public:
	/// Указать заголовок окна.
	virtual void SetTitle(const String& title) = 0;
	/// Закрыть окно.
	virtual void Close() = 0;

	/// Создать область вывода, покрывающую всё окно.
	virtual ptr<Graphics::Output> CreateOutput() = 0;
};

END_INANITY_PLATFORM

#endif
