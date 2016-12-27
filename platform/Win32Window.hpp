#ifndef ___INANITY_PLATFORM_WIN32_WINDOW_HPP___
#define ___INANITY_PLATFORM_WIN32_WINDOW_HPP___

#include "Window.hpp"
#include "../String.hpp"
#include "../Handler.hpp"
#include "../input/input.hpp"

#ifndef ___INANITY_PLATFORM_WINDOWS
#error Win32Window is implemented only on Windows.
#endif

#include "windows.hpp"

BEGIN_INANITY_INPUT

class Win32Manager;

END_INANITY_INPUT

BEGIN_INANITY_PLATFORM

class Win32Window : public Window
{
private:
	/// Дескриптор окна.
	HWND hWnd;
	/// Persistent HDC.
	HDC hdc;
	/// Destroy window in destructor.
	bool own;
	/// Активность окна.
	bool active;
	/// Клиентские размеры окна.
	int clientWidth, clientHeight;

	WNDPROC prevWndProc;

	ptr<Input::Win32Manager> inputManager;

	class Win32Icon;
	class Win32Cursor;

	/// Current icon.
	ptr<Win32Icon> icon;
	/// Is cursor was hided.
	/** This needed to not to break things, because
	ShowCursor() works like a counter. */
	bool cursorHidden;
	/// Current cursor.
	ptr<Win32Cursor> cursor;

	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	static ptr<Win32Window> Create(ATOM windowClass, const String& title,
		int left, int top, int width, int height, bool visible);

	/// Выполнить одну итерацию оконного цикла.
	bool Do(Handler* activeHandler);

	void UpdateMouseLock();
	void UpdateCursorVisible();
	void UpdateCursor();

	static HICON CreateIconFromTexture(ptr<Graphics::RawTextureData> texture, BOOL isIcon, int hotX, int hotY);

public:
	Win32Window(HWND hWnd, bool own = true, WNDPROC prevWndProc = nullptr);
	~Win32Window();

	//*** методы Window
	void SetTitle(const String& title);
	void Close();
	void Run(ptr<Handler> activeHandler);
	void PlaceCursor(int x, int y);
	void StartTextInput();
	void StopTextInput();
	ptr<Icon> CreateIcon(ptr<Graphics::RawTextureData> texture);
	void SetIcon(ptr<Icon> icon);
	ptr<Cursor> CreateCursor(ptr<Graphics::RawTextureData> texture, int hotX, int hotY);
	void SetCursor(ptr<Cursor> cursor);

	/// Создать окно для DirectX.
	static ptr<Win32Window> CreateForDirectX(const String& title,
		int left, int top, int width, int height, bool visible = true);
	/// Создать окно для OpenGL.
	static ptr<Win32Window> CreateForOpenGL(const String& title,
		int left, int top, int width, int height, bool visible = true);
	/// Capture existing window.
	static ptr<Win32Window> CreateExisting(HWND hWnd, bool own);

	/// Получить хендл окна.
	HWND GetHWND() const;
	/// Get a window's persistent HDC.
	HDC GetHDC() const;
	/// Получить активность окна.
	bool IsActive() const;
	/// Получить клиентскую ширину окна.
	int GetClientWidth() const;
	/// Получить клиентскую высоту окна.
	int GetClientHeight() const;

	/// Установить менеджер ввода.
	void SetInputManager(ptr<Input::Win32Manager> inputManager);
};

END_INANITY_PLATFORM

#endif
