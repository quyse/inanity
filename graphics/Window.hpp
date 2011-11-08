#ifndef ___INANITY_GRAPHICS_WINDOW_HPP___
#define ___INANITY_GRAPHICS_WINDOW_HPP___

#include "graphics.hpp"
#include "System.hpp"
#include "../String.hpp"
#include "../input/input.hpp"

#ifdef ___INANITY_WINDOWS

#include "../windows.hpp"

BEGIN_INANITY_INPUT

class Manager;

END_INANITY_INPUT

BEGIN_INANITY_GRAPHICS

class Window
{
private:
	//дескриптор окна
	HWND hWnd;
	//активность окна
	bool active;

	/// Единственный экземпляр главного окна.
	/** больше одного не разрешается, для ускорения обработки сообщений */
	static Window* singleWindow;

	/// Графическая система.
	ptr<System> graphicsSystem;
	/// Менеджер ввода.
	ptr<Input::Manager> inputManager;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	Window(const String& windowTitle);
	~Window();

	//получить окно
	HWND GetWindowHandle() const;
	//получить активность окна
	bool IsActive() const;

	/// Указать графику.
	void SetGraphicsSystem(ptr<System> graphicsSystem);
	/// Указать менеджер ввода.
	void SetInputManager(ptr<Input::Manager> inputManager);
};

END_INANITY_GRAPHICS

#else

#error Other than Windows Window is not implemented.

#endif

#endif
