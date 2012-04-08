#include "FreeContainer.hpp"
#include "Visualizer.hpp"
#include "Notificator.hpp"
#include <algorithm>

FreeContainer::FreeContainer(ptr<Visualizer> visualizer) : ContainerElement(visualizer)
{
}

void FreeContainer::AddElement(ptr<Element> element)
{
	elements.push_back(element);
	element->SetParent(this);
}

void FreeContainer::RemoveElement(ptr<Element> element)
{
	std::vector<ptr<Element> >::iterator i = std::find(elements.begin(), elements.end(), element);
	if(i != elements.end())
	{
		elements.erase(i);
		element->SetParent(nullptr);
	}
}

void FreeContainer::Draw(Position offset)
{
	offset += position;
	for(size_t i = 0; i < elements.size(); ++i)
		elements[i]->Draw(offset);
}

bool FreeContainer::IsPositionInto(Position position) const
{
	return ContainerElement::IsPositionInto(position) && GetElementByPosition(position);
}

ptr<Element> FreeContainer::GetElementByPosition(Position position) const
{
	for(size_t i = 0; i < elements.size(); ++i)
		if(elements[i]->IsPositionInto(position - elements[i]->GetPosition()))
			return elements[i];
	return nullptr;
}
