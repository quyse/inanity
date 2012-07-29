#include "Win32Window.hpp"
#include "System.hpp"
#include "../input/Manager.hpp"
#include "../Strings.hpp"
#include "../Exception.hpp"
#include "graphics/Win32Output.hpp"
#include <windowsx.h>

Win32Window* Win32Window::singleWindow = 0;

Win32Window::Win32Window(ATOM windowClass, const String& title) : active(true)
{
	try
	{
		if(singleWindow)
			THROW_PRIMARY_EXCEPTION("Can't create second game window");

		//создать окно
		int primaryWidth = GetSystemMetrics(SM_CXSCREEN);
		int primaryHeight = GetSystemMetrics(SM_CYSCREEN);
		hWnd = CreateWindow((LPCTSTR)windowClass, Strings::UTF82Unicode(windowTitle).c_str(), WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 1, 1, NULL, NULL, GetModuleHandle(NULL), NULL);
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

Win32Window::~Win32Window()
{
	if(hWnd) DestroyWindow(hWnd);
	singleWindow = 0;
}

void Win32Window::SetTitle(const String& title)
{
	SetWindowText(hWnd, title.c_str());
}

ptr<Graphics::Output> Win32Window::CreateOutput()
{
	return NEW(Graphics::Win32Output(this));
}

ptr<Win32Window> Win32Window::CreateForDirectX()
{
	static ATOM windowClass = NULL;
	//зарегистрировать класс окна, если еще не сделано
	if(!windowClass)
	{
		WNDCLASS wndClass;
		ZeroMemory(&wndClass, sizeof(wndClass));
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground = GetStockBrush(BLACK_BRUSH);
		wndClass.lpszClassName = TEXT("Win32DirectXWindow");
		wndClass.hInstance = GetModuleHandle(NULL);
		wndClass.lpfnWndProc = WndProc;
		windowClass = RegisterClass(&wndClass);
		if(!windowClass)
			THROW_PRIMARY_EXCEPTION("Can't register window class for DirectX");
	}

	return NEW(Win32Window(windowClass));
}

ptr<Win32Window> Win32Window::CreateForOpenGL()
{
	static ATOM windowClass = NULL;
	//зарегистрировать класс окна, если еще не сделано
	if(!windowClass)
	{
		WNDCLASS wndClass;
		ZeroMemory(&wndClass, sizeof(wndClass));
		wndClass.style = CS_OWNDC;
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.lpszClassName = TEXT("Win32OpenGLWindow");
		wndClass.hInstance = GetModuleHandle(NULL);
		wndClass.lpfnWndProc = WndProc;
		windowClass = RegisterClass(&wndClass);
		if(!windowClass)
			THROW_PRIMARY_EXCEPTION("Can't register window class for OpenGL");
	}

	return NEW(Win32Window(windowClass));
}

HWND Win32Window::GetWindowHandle() const
{
	return hWnd;
}

bool Win32Window::IsActive() const
{
	return active;
}

LRESULT CALLBACK Win32Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		singleWindow->Close();
		return 0;
	case WM_DESTROY:
		singleWindow->hWnd = 0;
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Win32Window::SetGraphicsSystem(ptr<Graphics::System> graphicsSystem)
{
	this->graphicsSystem = graphicsSystem;
}

void Win32Window::SetInputManager(ptr<Input::Manager> inputManager)
{
	this->inputManager = inputManager;
}

bool Win32Window::Do(ActiveHandler* activeHandler)
{
	MSG msg;
	bool lastActive;
	while((lastActive = active) ? PeekMessage(&msg, 0, 0, 0, PM_REMOVE) : GetMessage(&msg, 0, 0, 0))
	{
		if(msg.message == WM_QUIT) return false;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if(active != lastActive)
		{
			if(inputManager)
				if(active)
					inputManager->AcquireDevices();
				else
					inputManager->UnacquireDevices();
		}
	}

	if(active)
	{
		if(inputManager)
			inputManager->Update();
		activeHandler->Fire(0);
	}
	else
		//если не активно, то значит, вышли из цикла по GetMessage(), и сообщение - WM_QUIT
		return false;

	return true;
}

void Win32Window::Close()
{
	if(hWnd)
		DestroyWindow(hWnd);
}

void Win32Window::Run(ptr<ActiveHandler> activeHandler)
{
	while(Do(activeHandler));
}
