#include "NxWindow.hpp"
#include "../input/NxManager.hpp"

BEGIN_INANITY_PLATFORM

NxWindow::NxWindow() {}

void NxWindow::SetInputManager(ptr<Input::NxManager> inputManager)
{
	this->inputManager = inputManager;
}

int NxWindow::GetClientWidth() const
{
	return clientWidth;
}

int NxWindow::GetClientHeight() const
{
	return clientHeight;
}

void NxWindow::SetTitle(const String& title)
{
}

void NxWindow::Close()
{
}

void NxWindow::SetFullScreen(bool fullscreen)
{
}

float NxWindow::GetDPIScale() const
{
	return dpiScale;
}

void NxWindow::GetRect(int& left, int& top, int& width, int& height)
{
}

void NxWindow::Run(ptr<Handler> activeHandler)
{
}

void NxWindow::PlaceCursor(int x, int y)
{
}

ptr<Window::Icon> NxWindow::CreateIcon(ptr<Graphics::RawTextureData> texture)
{
	return nullptr; // FIX ME
}

void NxWindow::SetIcon(ptr<Icon> icon)
{
}

ptr<Window::Cursor> NxWindow::CreateCursor(ptr<Graphics::RawTextureData> texture, int hotX, int hotY)
{
	return nullptr; // FIX ME
}

void NxWindow::SetCursor(ptr<Cursor> cursor)
{
}


END_INANITY_PLATFORM
