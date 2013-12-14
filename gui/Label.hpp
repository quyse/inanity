#ifndef ___INANITY_GUI_LABEL_HPP___
#define ___INANITY_GUI_LABEL_HPP___

#include "Element.hpp"
#include "../String.hpp"

BEGIN_INANITY_GUI

/// Класс метки.
class Label : public Element
{
private:
	/// Текст метки.
	String text;

public:
	Label(ptr<Visualizer> visualizer);

	const String& GetText() const;
	void SetText(const String& text);

	void Draw(Canvas* canvas, Position offset);
};

END_INANITY_GUI

#endif
