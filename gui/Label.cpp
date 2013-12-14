#include "Label.hpp"
#include "Visualizer.hpp"
#include "Visuals.hpp"
#include "Notifications.hpp"

BEGIN_INANITY_GUI

Label::Label(ptr<Visualizer> visualizer)
: Element(visualizer) {}

const String& Label::GetText() const
{
	return text;
}

void Label::SetText(const String& text)
{
	this->text = text;
}

void Label::Draw(Canvas* canvas, Position offset)
{
	offset += position;
	visualizer->DrawRectText(canvas, VisualFonts::Label, text, offset.x, offset.y, offset.x + size.x, offset.y + size.y);
}

END_INANITY_GUI
