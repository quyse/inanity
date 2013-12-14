#include "Window.hpp"
#include "Visualizer.hpp"
#include "Notificator.hpp"

BEGIN_INANITY_GUI

Window::Window(ptr<Visualizer> visualizer, const String& title)
: ContentContainer(visualizer), title(title) {}

void Window::SetTitle(const String& title)
{
	this->title = title;
}

END_INANITY_GUI
