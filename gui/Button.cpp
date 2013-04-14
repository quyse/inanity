#include "Button.hpp"
#include "Visualizer.hpp"
#include "Visuals.hpp"
#include "Notifications.hpp"

BEGIN_INANITY_GUI

Button::Button(ptr<Visualizer> visualizer) : Element(visualizer), pressed(false)
{
}

const String& Button::GetText() const
{
	return text;
}

void Button::SetText(const String& text)
{
	this->text = text;
}

void Button::Draw(Visualizer* visualizer, Position offset)
{
	offset += position;
	visualizer->DrawVisualElement(pressed ? VisualElements::ButtonPressed : moused ? VisualElements::ButtonHighlighted : VisualElements::Button, offset.x, offset.y, offset.x + size.x, offset.y + size.y);
	visualizer->DrawRectText(pressed ? VisualFonts::ButtonPressed : moused ? VisualFonts::ButtonHighlighted : VisualFonts::Button, text, offset.x, offset.y, offset.x + size.x, offset.y + size.y);
}

void Button::EventMouseDown(Position position, MouseButton button)
{
	Element::EventMouseDown(position, button);
	pressed = true;
}

void Button::EventMouseUp(Position position, MouseButton button)
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
