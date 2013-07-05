#include "Win32Window.hpp"
#include "../input/Win32Manager.hpp"
#include "../Strings.hpp"
#include "../Exception.hpp"
#include "../graphics/Presenter.hpp"
#include "../graphics/Win32Output.hpp"
#include <windowsx.h>

BEGIN_INANITY_PLATFORM

Win32Window* Win32Window::singleWindow = 0;

Win32Window::Win32Window(ATOM windowClass, const String& title,
	int left, int top, int width, int height)
: active(true), graphicsPresenter(0), clientWidth(0), clientHeight(0)
{
	try
	{
		if(singleWindow)
			THROW_PRIMARY_EXCEPTION("Can't create second game window");

		//создать окно
		hWnd = CreateWindow(
			(LPCTSTR)windowClass, Strings::UTF82Unicode(title).c_str(),
			WS_OVERLAPPEDWINDOW | WS_VISIBLE /*WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS*/,
			left, top, width, height,
			NULL, NULL, GetModuleHandle(NULL), NULL);
		if(!hWnd)
			THROW_PRIMARY_EXCEPTION("Can't create window");
		ShowCursor(FALSE);

		RECT rect;
		GetClientRect(hWnd, &rect);
		clientWidth = rect.right - rect.left;
		clientHeight = rect.bottom - rect.top;

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
	SetWindowText(hWnd, Strings::UTF82Unicode(title).c_str());
}

ptr<Graphics::Output> Win32Window::CreateOutput()
{
	return NEW(Graphics::Win32Output(this));
}

ptr<Win32Window> Win32Window::CreateForDirectX(const String& title, int left, int top, int width, int height)
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

	return NEW(Win32Window(windowClass, title, left, top, width, height));
}

ptr<Win32Window> Win32Window::CreateForOpenGL(const String& title, int left, int top, int width, int height)
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

	return NEW(Win32Window(windowClass, title, left, top, width, height));
}

HWND Win32Window::GetHWND() const
{
	return hWnd;
}

bool Win32Window::IsActive() const
{
	return active;
}

int Win32Window::GetClientWidth() const
{
	return clientWidth;
}

int Win32Window::GetClientHeight() const
{
	return clientHeight;
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
		if(singleWindow)
		{
			singleWindow->clientWidth = LOWORD(lParam);
			singleWindow->clientHeight = HIWORD(lParam);
			if(singleWindow->graphicsPresenter)
				singleWindow->graphicsPresenter->Resize(singleWindow->clientWidth, singleWindow->clientHeight);
		}
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

void Win32Window::SetGraphicsPresenter(Graphics::Presenter* graphicsPresenter)
{
	this->graphicsPresenter = graphicsPresenter;
}

void Win32Window::SetInputManager(ptr<Input::Win32Manager> inputManager)
{
	this->inputManager = inputManager;
}

bool Win32Window::Do(ActiveHandler* activeHandler)
{
	/* По-видимому, PeekMessage может обрабатывать некоторые сообщения (в том числе WM_ACTIVATE)
	синхронно, не возвращая их в msg. Поэтому потом, определяя выход из GetMessage по WM_QUIT,
	нужно опираться на значение lastActive, так как active может поменяться внутри PeekMessage.
	*/

	MSG msg;
	bool lastActive;
	while((lastActive = active) ? PeekMessage(&msg, 0, 0, 0, PM_REMOVE) : GetMessage(&msg, 0, 0, 0))
	{
		if(msg.message == WM_QUIT)
			return false;
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

	// если окно активно, работаем
	if(active)
	{
		if(inputManager)
			inputManager->Update();
		activeHandler->Fire(0);
	}
	// иначе если окно неактивно, и вышли мы из цикла по GetMessage(), то это сообщение WM_QUIT
	else if(!lastActive)
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

END_INANITY_PLATFORM
