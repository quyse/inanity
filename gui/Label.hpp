#ifndef ___INANITY_GUI_LABEL_HPP___
#define ___INANITY_GUI_LABEL_HPP___

#include "Element.hpp"
#include "../String.hpp"

BEGIN_INANITY_GUI

class Visualizer;

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

	void Draw(Visualizer* visualizer, Position offset);
};

END_INANITY_GUI

#endif
