#ifndef ___INANITY_WIN32_WINDOW_HPP___
#define ___INANITY_WIN32_WINDOW_HPP___

#include "Window.hpp"
#include "String.hpp"
#include "Handler.hpp"
#include "input/input.hpp"

#ifndef ___INANITY_WINDOWS
#error Win32Window is implemented only on Windows.
#endif

#include "windows.hpp"

BEGIN_INANITY_GRAPHICS

class Presenter;

END_INANITY_GRAPHICS

BEGIN_INANITY_INPUT

class Win32Manager;

END_INANITY_INPUT

BEGIN_INANITY

class Win32Window : public Window
{
private:
	/// Дескриптор окна.
	HWND hWnd;
	/// Активность окна.
	bool active;

	/// Presenter для графики.
	Graphics::Presenter* graphicsPresenter;
	/// Менеждер ввода.
	ptr<Input::Win32Manager> inputManager;

	/// Единственный экземпляр главного окна.
	/** больше одного не разрешается, для ускорения обработки сообщений */
	static Win32Window* singleWindow;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/// Выполнить одну итерацию оконного цикла.
	bool Do(Handler* activeHandler);

public:
	Win32Window(ATOM windowClass, const String& title = String());
	~Win32Window();

	// методы Window
	void SetTitle(const String& title);
	void Close();
	ptr<Graphics::Output> CreateOutput();

	/// Создать окно для DirectX.
	static ptr<Win32Window> CreateForDirectX();
	/// Создать окно для OpenGL.
	static ptr<Win32Window> CreateForOpenGL();

	//получить окно
	HWND GetHWND() const;
	//получить активность окна
	bool IsActive() const;

	/// Установить presenter для оповещений.
	void SetGraphicsPresenter(Graphics::Presenter* graphicsPresenter);
	/// Установить менеджер ввода.
	void SetInputManager(ptr<Input::Win32Manager> inputManager);

	/// Запустить оконный цикл.
	void Run(ptr<Handler> activeHandler);
};

END_INANITY

#endif
