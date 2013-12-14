#include "ContentContainer.hpp"
#include "Visualizer.hpp"
#include "Notificator.hpp"

BEGIN_INANITY_GUI

ContentContainer::ContentContainer(ptr<Visualizer> visualizer)
: ContainerElement(visualizer) {}

void ContentContainer::DoLayout()
{
	if(contentElement)
	{
		contentElement->SetPosition(Position(0, 0));
		contentElement->SetSize(size);
	}
}

void ContentContainer::SetContent(ptr<Element> contentElement)
{
	this->contentElement = contentElement;
	DoLayout();
}

void ContentContainer::SetSize(Size size)
{
	ContainerElement::SetSize(size);
	DoLayout();
}

END_INANITY_GUI
