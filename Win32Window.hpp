#ifndef ___INANITY_WIN32_WINDOW_HPP___
#define ___INANITY_WIN32_WINDOW_HPP___

#include "Window.hpp"
#include "../String.hpp"
#include "../EventHandler.hpp"

#ifdef ___INANITY_WINDOWS

#include "windows.hpp"

BEGIN_INANITY

class Window : public Object
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
	Window(const String& windowTitle);
	~Window();

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
