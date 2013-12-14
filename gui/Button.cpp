#include "Button.hpp"
#include "Visualizer.hpp"
#include "Visuals.hpp"
#include "Notifications.hpp"

BEGIN_INANITY_GUI

Button::Button(ptr<Visualizer> visualizer)
: Element(visualizer), pressed(false) {}

const String& Button::GetText() const
{
	return text;
}

void Button::SetText(const String& text)
{
	this->text = text;
}

void Button::Draw(Canvas* canvas, Position offset)
{
	offset += position;
	visualizer->DrawVisualElement(canvas, pressed ? VisualElements::ButtonPressed : moused ? VisualElements::ButtonHighlighted : VisualElements::Button, offset.x, offset.y, offset.x + size.x, offset.y + size.y);
	visualizer->DrawRectText(canvas, pressed ? VisualFonts::ButtonPressed : moused ? VisualFonts::ButtonHighlighted : VisualFonts::Button, text, offset.x, offset.y, offset.x + size.x, offset.y + size.y);
}

void Button::EventMouseDown(Position position, MouseEvent::Button button)
{
	Element::EventMouseDown(position, button);
	pressed = true;
}

void Button::EventMouseUp(Position position, MouseEvent::Button button)
{
	Element::EventMouseUp(position, button);
	pressed = false;
	Notify(Notifications::Click);
}

void Button::EventMouseLeave()
{
	Element::EventMouseLeave();
	pressed = false;
}

END_INANITY_GUI
