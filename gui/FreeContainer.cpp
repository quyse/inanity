#include "FreeContainer.hpp"
#include "Visualizer.hpp"
#include "Notificator.hpp"
#include <algorithm>

BEGIN_INANITY_GUI

FreeContainer::FreeContainer(ptr<Visualizer> visualizer)
: ContainerElement(visualizer) {}

void FreeContainer::DoLayout(size_t number)
{
	const Place& place = elements[number].first;

	Position position;
	Size size;

	switch(place.horizontal.anchor)
	{
	case Place::anchorLeft:
		position.x = place.horizontal.left.left;
		size.x = place.horizontal.left.width;
		break;
	case Place::anchorRight:
		position.x = this->size.x - place.horizontal.right.right - place.horizontal.right.width;
		size.x = place.horizontal.right.width;
		break;
	case Place::anchorHorizontalStretch:
		position.x = place.horizontal.stretch.left;
		size.x = this->size.x - place.horizontal.stretch.left - place.horizontal.stretch.right;
		break;
	}

	switch(place.vertical.anchor)
	{
	case Place::anchorTop:
		position.y = place.vertical.top.top;
		size.y = place.vertical.top.height;
		break;
	case Place::anchorBottom:
		position.y = this->size.y - place.vertical.bottom.bottom - place.vertical.bottom.height;
		size.y = place.vertical.bottom.height;
		break;
	case Place::anchorVerticalStretch:
		position.y = place.vertical.stretch.top;
		size.y = this->size.y - place.vertical.stretch.top - place.vertical.stretch.bottom;
		break;
	}

	if(size.x < 0)
		size.x = 0;
	if(size.y < 0)
		size.y = 0;

	elements[number].second->SetPosition(position);
	elements[number].second->SetSize(size);
}

void FreeContainer::AddElement(const Place& place, ptr<Element> element)
{
	elements.push_back(std::make_pair(place, element));
	element->SetParent(this);
	DoLayout(elements.size() - 1);
}

void FreeContainer::RemoveElement(ptr<Element> element)
{
	for(size_t i = 0; i < elements.size(); ++i)
		if(elements[i].second == element)
		{
			elements.erase(elements.begin() + i);
			element->SetParent(nullptr);
			break;
		}
}

ptr<Element> FreeContainer::TryGetElementByPosition(Position position) const
{
	for(size_t i = 0; i < elements.size(); ++i)
		if(elements[i].second->IsPositionInto(position - elements[i].second->GetPosition()))
			return elements[i].second;
	return nullptr;
}

void FreeContainer::SetSize(Size size)
{
	ContainerElement::SetSize(size);

	for(size_t i = 0; i < elements.size(); ++i)
		DoLayout(i);
}

bool FreeContainer::IsPositionInto(Position position) const
{
	return ContainerElement::IsPositionInto(position) && TryGetElementByPosition(position);
}

void FreeContainer::Draw(Canvas* canvas, Position offset)
{
	offset += position;
	for(size_t i = 0; i < elements.size(); ++i)
		elements[i].second->Draw(canvas, offset);
}

END_INANITY_GUI
