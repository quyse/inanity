#include "Win32Window.hpp"
#include "../graphics/Presenter.hpp"
#include "../input/Win32Manager.hpp"
#include "../Strings.hpp"
#include "../Exception.hpp"
#include <windowsx.h>

BEGIN_INANITY_PLATFORM

Win32Window::Win32Window(HWND hWnd, bool own)
: hWnd(hWnd), own(own), active(true), clientWidth(0), clientHeight(0), cursorHidden(false)
{
	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);

	RECT rect;
	GetClientRect(hWnd, &rect);
	clientWidth = rect.right - rect.left;
	clientHeight = rect.bottom - rect.top;
}

Win32Window::~Win32Window()
{
	if(hWnd)
	{
		if(own)
			DestroyWindow(hWnd);
		else
			SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
	}
}

void Win32Window::SetTitle(const String& title)
{
	SetWindowText(hWnd, Strings::UTF82Unicode(title).c_str());
}

void Win32Window::PlaceCursor(int x, int y)
{
	POINT pt = { x, y };
	ClientToScreen(hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

ptr<Win32Window> Win32Window::Create(ATOM windowClass, const String& title,
	int left, int top, int width, int height)
{
	BEGIN_TRY();

	//создать окно
	HWND hWnd = CreateWindow(
		(LPCTSTR)windowClass, Strings::UTF82Unicode(title).c_str(),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE /*WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS*/,
		left, top, width, height,
		NULL, NULL, GetModuleHandle(NULL), NULL);
	if(!hWnd)
		THROW("Can't create window");

	return NEW(Win32Window(hWnd));

	END_TRY("Can't create game window");
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
		wndClass.lpfnWndProc = StaticWndProc;
		windowClass = RegisterClass(&wndClass);
		if(!windowClass)
			THROW("Can't register window class for DirectX");
	}

	return Create(windowClass, title, left, top, width, height);
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
		wndClass.lpfnWndProc = StaticWndProc;
		windowClass = RegisterClass(&wndClass);
		if(!windowClass)
			THROW("Can't register window class for OpenGL");
	}

	return Create(windowClass, title, left, top, width, height);
}

ptr<Win32Window> Win32Window::CreateExisting(HWND hWnd, bool own)
{
	SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)&StaticWndProc);
	return NEW(Win32Window(hWnd, own));
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

LRESULT CALLBACK Win32Window::StaticWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Win32Window* window = (Win32Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(window)
		return window->WndProc(uMsg, wParam, lParam);

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT Win32Window::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(inputManager && inputManager->ProcessWindowMessage(uMsg, wParam, lParam))
		return 0;

	switch(uMsg)
	{
	case WM_ACTIVATE:
		{
			unsigned state = LOWORD(wParam);
			bool active = (state == WA_ACTIVE || state == WA_CLICKACTIVE);
			// update activity flag
			active = active;
			// update mouse lock
			UpdateMouseLock();
			// update cursor visibility
			UpdateCursorVisible();
		}
		return 0;
	case WM_MOVE:
		UpdateMouseLock();
		return 0;
	case WM_SIZE:
		clientWidth = LOWORD(lParam);
		clientHeight = HIWORD(lParam);
		if(presenter)
			presenter->Resize(clientWidth, clientHeight);
		UpdateMouseLock();
		return 0;
	case WM_CLOSE:
		Close();
		return 0;
	case WM_DESTROY:
		hWnd = 0;
		ClipCursor(NULL);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Win32Window::SetInputManager(ptr<Input::Win32Manager> inputManager)
{
	this->inputManager = inputManager;
}

bool Win32Window::Do(Handler* activeHandler)
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
	}

	// если окно активно, работаем
	if(active)
	{
		if(inputManager)
			inputManager->Update();
		activeHandler->Fire();
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

void Win32Window::Run(ptr<Handler> activeHandler)
{
	while(Do(activeHandler));
}

void Win32Window::UpdateMouseLock()
{
	// get if we actually want to set mouse lock
	bool actualMouseLock = mouseLock && active;

	if(actualMouseLock)
	{
		// clip cursor into client rect in screen coordinates
		RECT rect;
		GetClientRect(hWnd, &rect);
		MapWindowPoints(hWnd, NULL, (LPPOINT)&rect, 2);
		ClipCursor(&rect);
	}
	else
		ClipCursor(NULL);
}

void Win32Window::UpdateCursorVisible()
{
	if(cursorVisible == cursorHidden)
	{
		ShowCursor(cursorVisible ? TRUE : FALSE);
		cursorHidden = !cursorVisible;
	}
}

END_INANITY_PLATFORM
