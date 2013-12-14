#ifndef ___INANITY_GUI_TEXT_BOX_HPP___
#define ___INANITY_GUI_TEXT_BOX_HPP___

#include "Element.hpp"
#include "../String.hpp"

BEGIN_INANITY_GUI

/// Text box class.
class TextBox : public Element
{
private:
	/// Text in the field, in UTF-8.
	String text;
	/// Position of selection begin, in UTF-8 bytes.
	size_t selectionBegin;
	/// Position of selection end, in UTF-8 bytes.
	size_t selectionEnd;
	/// Is shift key pressed.
	bool shiftPressed;

public:
	TextBox(ptr<Visualizer> visualizer);

	const String& GetText() const;
	void SetText(const String& text);

	void Draw(Canvas* canvas, Position offset);

	void EventMouseDown(Position position, MouseEvent::Button button);
	void EventKeyDown(Key key);
	void EventKeyUp(Key key);
	void EventKeyPress(wchar_t key);
};

END_INANITY_GUI

#endif
