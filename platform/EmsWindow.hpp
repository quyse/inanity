#ifndef ___INANITY_PLATFORM_EMS_WINDOW_HPP___
#define ___INANITY_PLATFORM_EMS_WINDOW_HPP___

#include "Window.hpp"
#include "../input/input.hpp"

#ifndef ___INANITY_PLATFORM_EMSCRIPTEN
#error EmsWindow is for Emscripten platform
#endif

BEGIN_INANITY_INPUT

class SdlManager;

END_INANITY_INPUT

BEGIN_INANITY_PLATFORM

class Sdl;

/// Dummy window class for Emscripten platform.
class EmsWindow : public Window
{
private:
	ptr<Sdl> sdl;
	String title;
	ptr<Handler> activeHandler;
	ptr<Input::SdlManager> inputManager;
	int width, height;

	/// Emscripten window can be only one.
	static EmsWindow* instance;

	/// Callback to emscripten to execute as a main loop.
	static void StaticMainLoop();
	void MainLoop();

	void UpdateMouseLock();
	void UpdateCursorVisible();

public:
	EmsWindow(const String& title, int width, int height);
	~EmsWindow();

	void SetInputManager(ptr<Input::SdlManager> inputManager);

	int GetWidth() const;
	int GetHeight() const;

	//*** Window's methods.
	void SetTitle(const String& title);
	void Close();
	void SetFullScreen(bool fullscreen);
	void GetRect(int& left, int& top, int& width, int& height);
	void Run(ptr<Handler> activeHandler);
	void PlaceCursor(int x, int y);
	void StartTextInput();
	void StopTextInput();
	ptr<Icon> CreateIcon(ptr<Graphics::RawTextureData> texture);
	void SetIcon(ptr<Icon> icon);
	ptr<Cursor> CreateCursor(ptr<Graphics::RawTextureData> texture, int hotX, int hotY);
	void SetCursor(ptr<Cursor> cursor);
};

END_INANITY_PLATFORM

#endif
