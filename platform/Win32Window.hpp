#ifndef ___INANITY_PLATFORM_WIN32_WINDOW_HPP___
#define ___INANITY_PLATFORM_WIN32_WINDOW_HPP___

#include "Window.hpp"
#include "../String.hpp"
#include "../Handler.hpp"
#include "../input/input.hpp"

#ifndef ___INANITY_WINDOWS
#error Win32Window is implemented only on Windows.
#endif

#include "../windows.hpp"

BEGIN_INANITY_GRAPHICS

class Win32Output;

END_INANITY_GRAPHICS

BEGIN_INANITY_INPUT

class Win32Manager;

END_INANITY_INPUT

BEGIN_INANITY_PLATFORM

class Win32Window : public Window
{
private:
	/// Дескриптор окна.
	HWND hWnd;
	/// Активность окна.
	bool active;
	/// Клиентские размеры окна.
	int clientWidth, clientHeight;

	/// Graphics output for window.
	Graphics::Win32Output* output;
	/// Менеждер ввода.
	ptr<Input::Win32Manager> inputManager;

	/// Единственный экземпляр главного окна.
	/** больше одного не разрешается, для ускорения обработки сообщений */
	static Win32Window* singleWindow;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/// Выполнить одну итерацию оконного цикла.
	bool Do(Handler* activeHandler);

public:
	Win32Window(ATOM windowClass, const String& title,
		int left = 0, int top = 0, int width = 1, int height = 1);
	~Win32Window();

	//*** методы Window
	void SetTitle(const String& title);
	void Close();
	void Run(ptr<Handler> activeHandler);
	ptr<Graphics::Output> CreateOutput();

	/// Создать окно для DirectX.
	static ptr<Win32Window> CreateForDirectX(const String& title,
		int left, int top, int width, int height);
	/// Создать окно для OpenGL.
	static ptr<Win32Window> CreateForOpenGL(const String& title,
		int left, int top, int width, int height);

	/// Получить хендл окна.
	HWND GetHWND() const;
	/// Получить активность окна.
	bool IsActive() const;
	/// Получить клиентскую ширину окна.
	int GetClientWidth() const;
	/// Получить клиентскую высоту окна.
	int GetClientHeight() const;

	/// Set output for notifications.
	void SetOutput(Graphics::Win32Output* output);
	/// Установить менеджер ввода.
	void SetInputManager(ptr<Input::Win32Manager> inputManager);
};

END_INANITY_PLATFORM

#endif
