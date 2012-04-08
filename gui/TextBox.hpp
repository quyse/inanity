#ifndef ___INANITY_GUI_TEXT_BOX_HPP___
#define ___INANITY_GUI_TEXT_BOX_HPP___

#include "Element.hpp"
#include "../String.hpp"

BEGIN_INANITY_GUI

class Visualizer;

/// Класс текстового поля UI.
class TextBox : public Element
{
private:
	/// Текст в текстовом поле.
	String text;
	/// Начало выделения.
	size_t selectionBegin;
	/// Конец выделения.
	size_t selectionEnd;
	/// Нажатость Shift.
	bool shiftPressed;

public:
	TextBox(ptr<Visualizer> visualizer);

	const String& GetText() const;
	void SetText(const String& text);

	void Draw(Visualizer* visualizer, Position offset);

	void EventMouseDown(Position position, MouseButton button);
	void EventKeyDown(Key key);
	void EventKeyUp(Key key);
	void EventKeyPress(wchar_t key);
};

END_INANITY_GUI

#endif
