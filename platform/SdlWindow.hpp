#ifndef ___INANITY_PLATFORM_SDL_WINDOW_HPP___
#define ___INANITY_PLATFORM_SDL_WINDOW_HPP___

#include "Window.hpp"
#include "../input/input.hpp"
#include <SDL2/SDL_video.h>

BEGIN_INANITY_INPUT

class SdlManager;

END_INANITY_INPUT

BEGIN_INANITY_PLATFORM

class Sdl;

/// Window class working via SDL.
class SdlWindow : public Window
{
private:
	ptr<Sdl> sdl;
	SDL_Window* handle;
	ptr<Input::SdlManager> inputManager;
	bool fullscreen;
	int virtualWidth, virtualHeight;
	int clientWidth, clientHeight;
	float dpiScale;

	class SdlIcon;
	class SdlCursor;

	static SDL_Surface* CreateSurfaceFromTexture(ptr<Graphics::RawTextureData> texture);

protected:
	void UpdateMouseLock();
	void UpdateCursorVisible();

public:
	SdlWindow(SDL_Window* handle);
	~SdlWindow();

	SDL_Window* GetHandle() const;

	void SetInputManager(ptr<Input::SdlManager> inputManager);

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
