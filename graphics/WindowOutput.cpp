#include "WindowOutput.hpp"
#include "Presenter.hpp"
#include "../platform/Window.hpp"

BEGIN_INANITY_GRAPHICS

WindowOutput::WindowOutput(ptr<Platform::Window> window, int width, int height)
: window(window), width(width), height(height) {}

ptr<Platform::Window> WindowOutput::GetWindow() const
{
	return window;
}

int WindowOutput::GetWidth() const
{
	return width;
}

int WindowOutput::GetHeight() const
{
	return height;
}

void WindowOutput::Resize(int width, int height)
{
	this->width = width;
	this->height = height;
	if(presenter)
		presenter->Resize(width, height);
}

END_INANITY_GRAPHICS
