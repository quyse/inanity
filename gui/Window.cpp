#include "Window.hpp"
#include "Visualizer.hpp"
#include "Notificator.hpp"

BEGIN_INANITY_GUI

Window::Window(ptr<Visualizer> visualizer, const String& title)
: ContentContainer(visualizer), title(title) {}

void Window::DoLayout()
{
	if(contentElement)
	{
		Position leftTop = visualizer->GetMetrics().windowClientLeftTop;
		Position rightBottom = visualizer->GetMetrics().windowClientRightBottom;
		contentElement->SetPosition(leftTop);
		contentElement->SetSize(size - leftTop - rightBottom);
	}
}

void Window::SetTitle(const String& title)
{
	this->title = title;
}

END_INANITY_GUI
