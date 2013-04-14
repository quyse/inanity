#include "Element.hpp"
#include "Visualizer.hpp"
#include "ContainerElement.hpp"

BEGIN_INANITY_GUI

Element::Element(ptr<Visualizer> visualizer)
	: visualizer(visualizer), parent(0), position(0, 0), size(1, 1), focused(false), moused(false)
{
}

void Element::Notify(int code)
{
	if(parent)
		parent->Notify(this, code);
}

ContainerElement* Element::GetParent() const
{
	return parent;
}

void Element::SetParent(ContainerElement* parent)
{
	this->parent = parent;
}

Position Element::GetPosition() const
{
	return position;
}

void Element::SetPosition(Position position)
{
	this->position = position;
}

Size Element::GetSize() const
{
	return size;
}

void Element::SetSize(Size size)
{
	this->size = size;
}

bool Element::IsFocused() const
{
	return focused;
}

void Element::SetFocus()
{
	//если мы еще не в фокусе
	if(!focused)
	{
		//если есть родительский элемент
		if(parent)
			//запросить у него фокус
			parent->QueryFocus(this);
		//иначе мы являемся корневым элементом, и ничего запрашивать не надо
		else
			EventSetFocus();
	}
}

bool Element::IsPositionInto(Position position) const
{
	return position.x >= 0 && position.y >= 0 && position.x < size.x && position.y < size.y;
}

void Element::EventMouseMove(Position position)
{
}

void Element::EventMouseDown(Position position, MouseButton button)
{
}

void Element::EventMouseUp(Position position, MouseButton button)
{
}

void Element::EventMouseEnter()
{
	moused = true;
}

void Element::EventMouseLeave()
{
	moused = false;
}

void Element::EventSetFocus()
{
	focused = true;
}

void Element::EventLostFocus()
{
	focused = false;
}

void Element::EventKeyDown(Key key)
{
}

void Element::EventKeyUp(Key key)
{
}

void Element::EventKeyPress(wchar_t key)
{
}

END_INANITY_GUI
