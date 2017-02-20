#include "Win32Window.hpp"
#include "../graphics/Presenter.hpp"
#include "../graphics/RawTextureData.hpp"
#include "../input/Win32Manager.hpp"
#include "../Strings.hpp"
#include "../Exception.hpp"
#include <windowsx.h>

BEGIN_INANITY_PLATFORM

typedef UINT (WINAPI *GETDPIFORWINDOW_FUNC)(HWND);

Win32Window::Win32Window(HWND hWnd, bool own, WNDPROC prevWndProc)
: hWnd(hWnd), hdc(0), own(own), active(true), clientWidth(0), clientHeight(0), dpiScale(1), prevWndProc(prevWndProc), cursorHidden(false), fullscreen(false)
{
	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);

	RECT rect;
	GetClientRect(hWnd, &rect);
	clientWidth = rect.right - rect.left;
	clientHeight = rect.bottom - rect.top;

	// use multiple methods to determine initial DPI scale
	UINT dpi;
	// GetDpiForWindow is supported starting with Windows 10
	HMODULE user32dll = GetModuleHandle(TEXT("user32.dll"));
	GETDPIFORWINDOW_FUNC getDpiForWindow = (GETDPIFORWINDOW_FUNC)GetProcAddress(user32dll, "GetDpiForWindow");
	if(getDpiForWindow)
	{
		dpi = getDpiForWindow(hWnd);
	}
	else
	{
		// works in every Windows version, but returns DPI of primary monitor
		// which is a system-wide setting updated only on re-login
		HDC hdc = GetDC(hWnd);
		dpi = GetDeviceCaps(hdc, LOGPIXELSX);
		ReleaseDC(hWnd, hdc);
	}
	dpiScale = float(dpi) / float(USER_DEFAULT_SCREEN_DPI);
}

Win32Window::~Win32Window()
{
	if(hWnd)
	{
		if(hdc)
			ReleaseDC(hWnd, hdc);
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

HICON Win32Window::CreateIconFromTexture(ptr<Graphics::RawTextureData> texture, BOOL isIcon, int hotX, int hotY)
{
	int width = texture->GetImageWidth();
	int height = texture->GetImageHeight();
	int pitch = (width * 3 + 3) & ~3;
	BITMAPV5HEADER h;
	ZeroMemory(&h, sizeof(h));
	h.bV5Size = sizeof(h);
	h.bV5Width = width;
	h.bV5Height = height;
	h.bV5Planes = 1;
	h.bV5BitCount = 32;
	h.bV5Compression = BI_BITFIELDS;
	h.bV5RedMask = 0x00FF0000;
	h.bV5GreenMask = 0x0000FF00;
	h.bV5BlueMask = 0x000000FF;
	h.bV5AlphaMask = 0xFF000000;
	const uint8_t* pixels = (const uint8_t*)texture->GetMipData();
	uint8_t* buf = new uint8_t[width * 4 * height];
	for(int i = 0; i < height; ++i)
	{
		const uint8_t* linePixels = pixels + i * width * 4;
		uint8_t* lineBuf = buf + (height - 1 - i) * width * 4;
		for(int j = 0; j < width; ++j)
		{
			lineBuf[j * 4 + 0] = linePixels[j * 4 + 2];
			lineBuf[j * 4 + 1] = linePixels[j * 4 + 1];
			lineBuf[j * 4 + 2] = linePixels[j * 4 + 0];
			lineBuf[j * 4 + 3] = linePixels[j * 4 + 3];
		}
	}

	HDC hdc = GetDC(NULL);
	HBITMAP hbmpColor = CreateDIBitmap(hdc, (BITMAPINFOHEADER*)&h, CBM_INIT, buf, (BITMAPINFO*)&h, DIB_RGB_COLORS);
	HBITMAP hbmpMask = CreateBitmap(width, height, 1, 1, NULL);
	ReleaseDC(NULL, hdc);

	delete [] buf;

	if(!hbmpColor || !hbmpMask)
	{
		if(hbmpColor) DeleteBitmap(hbmpColor);
		if(hbmpMask) DeleteBitmap(hbmpMask);
		THROW("Can't create bitmaps");
	}

	ICONINFO ii;
	ii.fIcon = isIcon;
	ii.xHotspot = hotX;
	ii.yHotspot = hotY;
	ii.hbmMask = hbmpMask;
	ii.hbmColor = hbmpColor;
	HICON icon = CreateIconIndirect(&ii);

	DeleteBitmap(hbmpColor);
	DeleteBitmap(hbmpMask);

	return icon;
}

class Win32Window::Win32Icon : public Window::Icon
{
	friend class Win32Window;
private:
	HICON icon;

public:
	Win32Icon(HICON icon) : icon(icon) {}
	~Win32Icon()
	{
		DestroyIcon(icon);
	}
};

ptr<Window::Icon> Win32Window::CreateIcon(ptr<Graphics::RawTextureData> texture)
{
	BEGIN_TRY();

	if(!(texture->GetFormat() == Graphics::PixelFormats::uintRGBA32S))
		THROW("Win32 window icon must be RGBA");

	HCURSOR icon = CreateIconFromTexture(texture, TRUE, 0, 0);

	if(!icon) THROW("Can't create icon");

	return NEW(Win32Icon(icon));

	END_TRY("Can't create Win32 icon");
}

void Win32Window::SetIcon(ptr<Icon> icon)
{
	ptr<Win32Icon> win32Icon = icon.FastCast<Win32Icon>();
	HICON ico = win32Icon->icon;
	SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)ico);
	SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)ico);
	this->icon = win32Icon;
}

class Win32Window::Win32Cursor : public Window::Cursor
{
	friend class Win32Window;
private:
	HCURSOR cursor;

public:
	Win32Cursor(HCURSOR cursor) : cursor(cursor) {}
	~Win32Cursor()
	{
		DestroyCursor(cursor);
	}
};

ptr<Window::Cursor> Win32Window::CreateCursor(ptr<Graphics::RawTextureData> texture, int hotX, int hotY)
{
	BEGIN_TRY();

	if(!(texture->GetFormat() == Graphics::PixelFormats::uintRGBA32S))
		THROW("Win32 window cursor must be RGB");

	HCURSOR cursor = CreateIconFromTexture(texture, FALSE, hotX, hotY);

	if(!cursor) THROW("Can't create cursor");

	return NEW(Win32Cursor(cursor));

	END_TRY("Can't create Win32 cursor");
}

void Win32Window::SetCursor(ptr<Cursor> cursor)
{
	ptr<Win32Cursor> lastCursor = this->cursor;
	this->cursor = cursor.FastCast<Win32Cursor>();
	UpdateCursor();
}

ptr<Win32Window> Win32Window::Create(ATOM windowClass, const String& title,
	int left, int top, int width, int height, bool visible)
{
	BEGIN_TRY();

	//создать окно
	HWND hWnd = CreateWindow(
		(LPCTSTR)windowClass, Strings::UTF82Unicode(title).c_str(),
		WS_OVERLAPPEDWINDOW | (visible ? WS_VISIBLE : 0),
		left, top, width, height,
		NULL, NULL, GetModuleHandle(NULL), NULL);
	if(!hWnd)
		THROW("Can't create window");

	return NEW(Win32Window(hWnd));

	END_TRY("Can't create game window");
}

ptr<Win32Window> Win32Window::CreateForDirectX(const String& title, int left, int top, int width, int height, bool visible)
{
	static ATOM windowClass = NULL;
	//зарегистрировать класс окна, если еще не сделано
	if(!windowClass)
	{
		WNDCLASS wndClass;
		ZeroMemory(&wndClass, sizeof(wndClass));
		wndClass.style = CS_DBLCLKS;
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground = GetStockBrush(BLACK_BRUSH);
		wndClass.lpszClassName = TEXT("Win32DirectXWindow");
		wndClass.hInstance = GetModuleHandle(NULL);
		wndClass.lpfnWndProc = StaticWndProc;
		windowClass = RegisterClass(&wndClass);
		if(!windowClass)
			THROW("Can't register window class for DirectX");
	}

	return Create(windowClass, title, left, top, width, height, visible);
}

ptr<Win32Window> Win32Window::CreateForOpenGL(const String& title, int left, int top, int width, int height, bool visible)
{
	BEGIN_TRY();

	static ATOM windowClass = NULL;
	//зарегистрировать класс окна, если еще не сделано
	if(!windowClass)
	{
		WNDCLASS wndClass;
		ZeroMemory(&wndClass, sizeof(wndClass));
		wndClass.style = CS_DBLCLKS | CS_OWNDC;
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.lpszClassName = TEXT("Win32OpenGLWindow");
		wndClass.hInstance = GetModuleHandle(NULL);
		wndClass.lpfnWndProc = StaticWndProc;
		windowClass = RegisterClass(&wndClass);
		if(!windowClass)
			THROW("Can't register window class");
	}

	ptr<Win32Window> window = Create(windowClass, title, left, top, width, height, visible);

	// get window's persistent HDC
	HDC hdc = GetDC(window->GetHWND());
	if(!hdc)
		THROW_SECONDARY("Can't get window's HDC", Exception::SystemError());
	// store it to window
	window->hdc = hdc;

	// choose & set pixel format
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 0;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int pixelFormat = ChoosePixelFormat(hdc, &pfd);
	if(!pixelFormat)
		THROW("Can't choose pixel format");
	if(!SetPixelFormat(hdc, pixelFormat, &pfd))
		THROW("Can't set pixel format");

	return window;

	END_TRY("Can't create window for OpenGL");
}

ptr<Win32Window> Win32Window::CreateExisting(HWND hWnd, bool own)
{
	WNDPROC prevWndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)&StaticWndProc);
	return NEW(Win32Window(hWnd, own, prevWndProc));
}

HWND Win32Window::GetHWND() const
{
	return hWnd;
}

HDC Win32Window::GetHDC() const
{
	return hdc;
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
			this->active = active;
			// update mouse lock
			UpdateMouseLock();
			// update cursor visibility
			UpdateCursorVisible();
			// tell input manager that window has been deactivated
			if(!active && inputManager)
				inputManager->ReleaseButtonsOnUpdate();
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
	case WM_SETCURSOR:
		if(LOWORD(lParam) == HTCLIENT)
		{
			UpdateCursor();
			return 0;
		}
		break;
	case WM_DPICHANGED:
		{
			dpiScale = float(LOWORD(wParam)) / float(USER_DEFAULT_SCREEN_DPI);
			const RECT* rect = (const RECT*)lParam;
			SetWindowPos(hWnd, NULL, rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top, SWP_NOACTIVATE | SWP_NOZORDER);
		}
		break;
	case WM_SYSCOMMAND:
		// prevent use of Alt for system menu
		// see https://msdn.microsoft.com/en-us/library/windows/desktop/ms646360
		if(wParam == SC_KEYMENU && GET_Y_LPARAM(lParam) <= 0) return 0;
		break;
	case WM_CLOSE:
		Close();
		return 0;
	case WM_DESTROY:
		hWnd = 0;
		ClipCursor(NULL);
		if(own)
			PostQuitMessage(0);
		return 0;
	}

	if(prevWndProc)
		return CallWindowProc(prevWndProc, hWnd, uMsg, wParam, lParam);
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
	while(((lastActive = active) || !sleepWhenInactive) ? PeekMessage(&msg, 0, 0, 0, PM_REMOVE) : GetMessage(&msg, 0, 0, 0))
	{
		if(msg.message == WM_QUIT)
			return false;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// если окно активно, или сказано не спать, работаем
	if(active || !sleepWhenInactive)
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

void Win32Window::SetFullScreen(bool fullscreen)
{
	if(this->fullscreen == fullscreen) return;
	this->fullscreen = fullscreen;
	if(fullscreen)
	{
		// remember window placement before going fullscreen
		LONG_PTR styles = GetWindowLongPtr(hWnd, GWL_STYLE);
		preFullScreenPlacement.length = sizeof(preFullScreenPlacement);
		GetWindowPlacement(hWnd, &preFullScreenPlacement);
		// remove window frame
		SetWindowLongPtr(hWnd, GWL_STYLE, (styles & (~WS_OVERLAPPEDWINDOW)) | WS_POPUP);
		// if window is maximized already, restore it first
		// otherwise Windows will not maximize it over whole screen
		if(styles & WS_MAXIMIZE) ShowWindowAsync(hWnd, SW_RESTORE);
		// maximize window
		ShowWindowAsync(hWnd, SW_MAXIMIZE);
	}
	else
	{
		// bring window frame back
		SetWindowLongPtr(hWnd, GWL_STYLE, (GetWindowLongPtr(hWnd, GWL_STYLE) & (~WS_POPUP)) | WS_OVERLAPPEDWINDOW);
		// restore window placement
		SetWindowPlacement(hWnd, &preFullScreenPlacement);
	}
}

float Win32Window::GetDPIScale() const
{
	return dpiScale;
}

void Win32Window::GetRect(int& left, int& top, int& width, int& height)
{
	RECT rect;
	GetWindowRect(hWnd, &rect);
	left = rect.left;
	top = rect.top;
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
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

void Win32Window::UpdateCursor()
{
	::SetCursor(cursor->cursor);
}

END_INANITY_PLATFORM
