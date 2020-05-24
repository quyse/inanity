#include "NxWindow.hpp"
#include "../input/NxManager.hpp"

BEGIN_INANITY_PLATFORM

NxWindow::NxWindow()
: clientWidth(1280), clientHeight(720), dpiScale(1) // TODO
{}

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
	Stop();
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
	while(running)
	{
		if(inputManager)
			inputManager->Update();
		activeHandler->Fire();
	}
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

void NxWindow::UpdateMouseLock()
{
}

void NxWindow::UpdateCursorVisible()
{
}

END_INANITY_PLATFORM
