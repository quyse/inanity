#include "Window.hpp"
#include "System.hpp"
#include "../input/Manager.hpp"
#include "../Strings.hpp"
#include "../Exception.hpp"
#include <windowsx.h>

Window* Window::singleWindow = 0;

Window::Window(const String& windowTitle) : active(true)
{
	try
	{
		if(singleWindow)
			THROW_PRIMARY_EXCEPTION("Can't create second game window");

		static ATOM windowClass = NULL;
		//зарегистрировать класс окна, если еще не сделано
		if(!windowClass)
		{
			WNDCLASS wndClass;
			ZeroMemory(&wndClass, sizeof(wndClass));
			wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
			wndClass.hbrBackground = GetStockBrush(BLACK_BRUSH);
			wndClass.lpszClassName = TEXT("Window");
			wndClass.hInstance = GetModuleHandle(NULL);
			wndClass.lpfnWndProc = WndProc;
			windowClass = RegisterClass(&wndClass);
			if(!windowClass)
				THROW_PRIMARY_EXCEPTION("Can't register window class");
		}
		//создать окно
		int primaryWidth = GetSystemMetrics(SM_CXSCREEN);
		int primaryHeight = GetSystemMetrics(SM_CYSCREEN);
		hWnd = CreateWindow((LPCTSTR)windowClass, Strings::UTF82Unicode(windowTitle).c_str(), WS_POPUP | WS_VISIBLE, 0, 0, 1, 1, NULL, NULL, GetModuleHandle(NULL), NULL);
		if(!hWnd)
			THROW_PRIMARY_EXCEPTION("Can't create window");
		ShowCursor(FALSE);

		singleWindow = this;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create game window", exception);
	}
}

Window::~Window()
{
	if(hWnd) DestroyWindow(hWnd);
	singleWindow = 0;
}

HWND Window::GetWindowHandle() const
{
	return hWnd;
}

bool Window::IsActive() const
{
	return active;
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(singleWindow && singleWindow->inputManager)
		if(singleWindow->inputManager->ProcessWindowMessage(uMsg, wParam, lParam))
			return 0;
	switch(uMsg)
	{
	case WM_ACTIVATE:
		{
			unsigned state = LOWORD(wParam);
			bool active = (state == WA_ACTIVE || state == WA_CLICKACTIVE);
			//обновить флаг активности
			if(singleWindow)
				singleWindow->active = active;
			//включить или отключить ограничение курсора мыши
			if(active)
			{
				RECT rect;
				GetWindowRect(hWnd, &rect);
				ClipCursor(&rect);
			}
			else
				ClipCursor(NULL);
		}
		return 0;
	case WM_MOVE:
		return 0;
	case WM_SIZE:
		if(singleWindow && singleWindow->graphicsSystem)
			singleWindow->graphicsSystem->Resize(LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 0;
	case WM_DESTROY:
		singleWindow->hWnd = 0;
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Window::SetGraphicsSystem(ptr<System> graphicsSystem)
{
	this->graphicsSystem = graphicsSystem;
}

void Window::SetInputManager(ptr<Input::Manager> inputManager)
{
	this->inputManager = inputManager;
}
