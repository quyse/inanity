#ifndef ___INANITY_WINDOW_HPP___
#define ___INANITY_WINDOW_HPP___

#include "String.hpp"
#include "graphics/graphics.hpp"

BEGIN_INANITY_GRAPHICS

class Output;

END_INANITY_GRAPHICS

BEGIN_INANITY

/// Абстрактный класс окна.
class Window : public Object
{
public:
	/// Указать заголовок окна.
	virtual void SetTitle(const String& title) = 0;

	/// Создать область вывода.
	virtual ptr<Graphics::Output> CreateOutput() = 0;
};

END_INANITY

#endif
