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
	void SetTitle(const String& title) override;
	void Close() override;
	void SetFullScreen(bool fullscreen)  override;
	float GetDPIScale() const override;
	void GetRect(int& left, int& top, int& width, int& height) override;
	void Run(ptr<Handler> activeHandler) override;
	void PlaceCursor(int x, int y) override;
	ptr<Icon> CreateIcon(ptr<Graphics::RawTextureData> texture) override;
	void SetIcon(ptr<Icon> icon) override;
	ptr<Window::Cursor> CreateCursor(ptr<Graphics::RawTextureData> texture, int hotX, int hotY) override;
	void SetCursor(ptr<Cursor> cursor) override;
};

END_INANITY_PLATFORM

#endif
