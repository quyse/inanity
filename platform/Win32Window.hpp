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
	/// Destroy window in destructor.
	bool own;
	/// Активность окна.
	bool active;
	/// Клиентские размеры окна.
	int clientWidth, clientHeight;

	WNDPROC prevWndProc;

	ptr<Input::Win32Manager> inputManager;

	/// Is cursor was hided.
	/** This needed to not to break things, because
	ShowCursor() works like a counter. */
	bool cursorHidden;

	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	static ptr<Win32Window> Create(ATOM windowClass, const String& title,
		int left = 0, int top = 0, int width = 1, int height = 1);

	/// Выполнить одну итерацию оконного цикла.
	bool Do(Handler* activeHandler);

	void UpdateMouseLock();
	void UpdateCursorVisible();

public:
	Win32Window(HWND hWnd, bool own = true, WNDPROC prevWndProc = nullptr);
	~Win32Window();

	//*** методы Window
	void SetTitle(const String& title);
	void Close();
	void Run(ptr<Handler> activeHandler);
	void PlaceCursor(int x, int y);

	/// Создать окно для DirectX.
	static ptr<Win32Window> CreateForDirectX(const String& title,
		int left, int top, int width, int height);
	/// Создать окно для OpenGL.
	static ptr<Win32Window> CreateForOpenGL(const String& title,
		int left, int top, int width, int height);
	/// Capture existing window.
	static ptr<Win32Window> CreateExisting(HWND hWnd, bool own);

	/// Получить хендл окна.
	HWND GetHWND() const;
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
