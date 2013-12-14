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
		Distance left = visualizer->GetMetric(VisualMetrics::WindowClientLeft);
		Distance top = visualizer->GetMetric(VisualMetrics::WindowClientTop);
		Distance right = visualizer->GetMetric(VisualMetrics::WindowClientRight);
		Distance bottom = visualizer->GetMetric(VisualMetrics::WindowClientBottom);
		contentElement->SetPosition(Position(left, top));
		contentElement->SetSize(Size(size.x - left - right, size.y - top - bottom));
	}
}

void Window::SetTitle(const String& title)
{
	this->title = title;
}

END_INANITY_GUI
