#include "X11Output.hpp"
#include "../platform/X11Window.hpp"
#include "../platform/X11Display.hpp"
#include "../graphics/Presenter.hpp"

BEGIN_INANITY_GRAPHICS

X11Output::X11Output(ptr<Platform::X11Window> window)
: window(window)
{
	window->SetOutput(this);

	::Window root;
	int x, y;
	unsigned int tempWidth, tempHeight, borderWidth, depth;
	XGetGeometry(window->GetDisplay()->GetDisplay(), window->GetHandle(),
		&root, &x, &y, &tempWidth, &tempHeight, &borderWidth, &depth);
	width = (int)tempWidth;
	height = (int)tempHeight;
}

X11Output::~X11Output()
{
	window->SetOutput(0);
}

int X11Output::GetWidth() const
{
	return width;
}

int X11Output::GetHeight() const
{
	return height;
}

ptr<Platform::X11Window> X11Output::GetWindow() const
{
	return window;
}

void X11Output::Resize(int width, int height)
{
	this->width = width;
	this->height = height;
	if(presenter)
		presenter->Resize(width, height);
}

END_INANITY_GRAPHICS
