#ifndef ___INANITY_PLATFORM_NX_WINDOW_HPP___
#define ___INANITY_PLATFORM_NX_WINDOW_HPP___

#include "Window.hpp"
#include "../input/input.hpp"

BEGIN_INANITY_INPUT

class NxManager;

END_INANITY_INPUT

BEGIN_INANITY_PLATFORM

/// Window class working via Nintendo SDK.
class NxWindow : public Window
{
private:
	ptr<Input::NxManager> inputManager;
	bool quit;
	bool fullscreen;
	int virtualWidth, virtualHeight;
	int clientWidth, clientHeight;
	float dpiScale;

protected:
	void UpdateMouseLock();
	void UpdateCursorVisible();

public:
	NxWindow();

	void SetInputManager(ptr<Input::NxManager> inputManager);

	int GetClientWidth() const;
	int GetClientHeight() const;

	//*** Window's methods.
	void SetTitle(const String& title);
	void Close();
	void SetFullScreen(bool fullscreen);
	float GetDPIScale() const;
	void GetRect(int& left, int& top, int& width, int& height);
	void Run(ptr<Handler> activeHandler);
	void PlaceCursor(int x, int y);
	ptr<Icon> CreateIcon(ptr<Graphics::RawTextureData> texture);
	void SetIcon(ptr<Icon> icon);
	ptr<Window::Cursor> CreateCursor(ptr<Graphics::RawTextureData> texture, int hotX, int hotY);
	void SetCursor(ptr<Cursor> cursor);
};

END_INANITY_PLATFORM

#endif
