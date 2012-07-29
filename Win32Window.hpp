#ifndef ___INANITY_WIN32_WINDOW_HPP___
#define ___INANITY_WIN32_WINDOW_HPP___

#include "Window.hpp"
#include "../String.hpp"
#include "../EventHandler.hpp"

#ifdef ___INANITY_WINDOWS

#include "windows.hpp"

BEGIN_INANITY

class Win32Window : public Window
{
public:
	/// Тип обработчика активного окна.
	typedef EventHandler<int> ActiveHandler;

private:
	//дескриптор окна
	HWND hWnd;
	//активность окна
	bool active;

	/// Единственный экземпляр главного окна.
	/** больше одного не разрешается, для ускорения обработки сообщений */
	static Window* singleWindow;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/// Выполнить одну итерацию оконного цикла.
	bool Do(ActiveHandler* activeHandler);

public:
	Win32Window(ATOM windowClass, const String& title = String());
	~Win32Window();

	// методы Window
	void SetTitle(const String& title);
	ptr<Graphics::Output> CreateOutput();

	/// Создать окно для DirectX.
	static ptr<Win32Window> CreateForDirectX();
	/// Создать окно для OpenGL.
	static ptr<Win32Window> CreateForOpenGL();

	//получить окно
	HWND GetWindowHandle() const;
	//получить активность окна
	bool IsActive() const;

	/// Закрыть окно.
	/** Также прекращает оконный цикл. */
	void Close();

	/// Запустить оконный цикл.
	void Run(ptr<ActiveHandler> activeHandler);
};

END_INANITY

#else

#error Win32Window is implemented only on Windows.

#endif

#endif
