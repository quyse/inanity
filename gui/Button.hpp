#ifndef ___INANITY_GUI_BUTTON_HPP___
#define ___INANITY_GUI_BUTTON_HPP___

#include "Element.hpp"
#include "../String.hpp"

BEGIN_INANITY_GUI

/// Класс кнопки UI.
class Button : public Element
{
private:
	/// Текст на кнопке.
	String text;
	/// Нажата ли кнопка?
	bool pressed;

public:
	Button();

	const String& GetText() const;
	void SetText(const String& text);

	void Draw(Visualizer* visualizer, Position offset);

	void EventMouseDown(Position position, MouseButton button);
	void EventMouseUp(Position position, MouseButton button);
	void EventMouseLeave();
};

END_INANITY_GUI

#endif
