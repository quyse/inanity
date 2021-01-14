#ifndef ___INANITY_PLATFORM_CORE_WINDOW_HPP___
#define ___INANITY_PLATFORM_CORE_WINDOW_HPP___

#include "Window.hpp"
#include "../input/input.hpp"
#include "windows.hpp"
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Core.h>

BEGIN_INANITY_INPUT

class CoreManager;

END_INANITY_INPUT

BEGIN_INANITY_PLATFORM

class CoreWindow : public Window
{
private:
	winrt::Windows::UI::Core::CoreWindow window;
	String title;
	ptr<Handler> activeHandler;
	bool active;

	ptr<Input::CoreManager> inputManager;

	class ViewProvider;
	class ViewProviderFactory;

	CoreWindow(const String& title);

	void UpdateMouseLock();
	void UpdateCursorVisible();

public:
	static ptr<CoreWindow> CreateForDirectX(const String& title);

	winrt::Windows::Foundation::IUnknown GetWindowUnknown() const;

	//*** Window's methods
	void SetTitle(const String& title);
	void Close();
	void SetFullScreen(bool fullscreen);
	float GetDPIScale() const;
	void GetRect(int& left, int& top, int& width, int& height);
	void Run(ptr<Handler> activeHandler);
	void PlaceCursor(int x, int y);
	ptr<Icon> CreateIcon(ptr<Graphics::RawTextureData> texture);
	void SetIcon(ptr<Icon> icon);
	ptr<Cursor> CreateCursor(ptr<Graphics::RawTextureData> texture, int hotX, int hotY);
	void SetCursor(ptr<Cursor> cursor);

	/// Установить менеджер ввода.
	void SetInputManager(ptr<Input::CoreManager> inputManager);
};

END_INANITY_PLATFORM

#endif
