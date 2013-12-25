#include "Win32RawManager.hpp"
#include "Frame.hpp"
#include "../Exception.hpp"
#include <algorithm>

BEGIN_INANITY_INPUT

Win32RawManager::Win32RawManager(HWND hWnd) : hWnd(hWnd)
{
	try
	{
		//зарегистрировать окно для принятия сообщений сырого ввода
		RAWINPUTDEVICE devices[2];
		//мышь
		devices[0].usUsagePage = 0x01;
		devices[0].usUsage = 0x02;
		devices[0].dwFlags = 0;//RIDEV_NOLEGACY;
		devices[0].hwndTarget = hWnd;
		//клавиатура
		devices[1].usUsagePage = 0x01;
		devices[1].usUsage = 0x06;
		devices[1].dwFlags = 0;//RIDEV_NOLEGACY;
		devices[1].hwndTarget = hWnd;
		if(!RegisterRawInputDevices(devices, 2, sizeof(devices[0])))
			THROW("Can't register window to receiving raw input");
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create raw input manager", exception);
	}
}

bool Win32RawManager::ProcessWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(Win32Manager::ProcessWindowMessage(msg, wParam, lParam))
		return true;
	if(msg == WM_INPUT)
	{
		RAWINPUT block;
		UINT bufferSize = sizeof(RAWINPUT);
		if(GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &block, &bufferSize, sizeof(RAWINPUTHEADER)))
			switch(block.header.dwType)
			{
			case RIM_TYPEKEYBOARD:
				{
					Event e;
					e.device = Event::deviceKeyboard;
					e.keyboard.type = !(block.data.keyboard.Flags & RI_KEY_BREAK) ? Event::Keyboard::typeKeyDown : Event::Keyboard::typeKeyUp;
					e.keyboard.key = ConvertKey(block.data.keyboard.VKey);
					AddEvent(e);
				}
				break;
			case RIM_TYPEMOUSE:
				{
					Event e;
					e.device = Event::deviceMouse;
					USHORT flags = block.data.mouse.usButtonFlags;

					if(flags & RI_MOUSE_LEFT_BUTTON_DOWN)
					{
						e.mouse.type = Event::Mouse::typeButtonDown;
						e.mouse.button = Event::Mouse::buttonLeft;
						AddEvent(e);
					}
					else if(flags & RI_MOUSE_LEFT_BUTTON_UP)
					{
						e.mouse.type = Event::Mouse::typeButtonUp;
						e.mouse.button = Event::Mouse::buttonLeft;
						AddEvent(e);
					}

					if(flags & RI_MOUSE_RIGHT_BUTTON_DOWN)
					{
						e.mouse.type = Event::Mouse::typeButtonDown;
						e.mouse.button = Event::Mouse::buttonRight;
						AddEvent(e);
					}
					else if(flags & RI_MOUSE_RIGHT_BUTTON_UP)
					{
						e.mouse.type = Event::Mouse::typeButtonUp;
						e.mouse.button = Event::Mouse::buttonRight;
						AddEvent(e);
					}

					if(flags & RI_MOUSE_MIDDLE_BUTTON_DOWN)
					{
						e.mouse.type = Event::Mouse::typeButtonDown;
						e.mouse.button = Event::Mouse::buttonMiddle;
						AddEvent(e);
					}
					else if(flags & RI_MOUSE_MIDDLE_BUTTON_UP)
					{
						e.mouse.type = Event::Mouse::typeButtonUp;
						e.mouse.button = Event::Mouse::buttonMiddle;
						AddEvent(e);
					}

					if(block.data.mouse.lLastX != 0 || block.data.mouse.lLastY != 0 || (flags & RI_MOUSE_WHEEL))
					{
						e.mouse.type = Event::Mouse::typeRawMove;
						e.mouse.rawMoveX = (float)block.data.mouse.lLastX;
						e.mouse.rawMoveY = (float)block.data.mouse.lLastY;
						e.mouse.rawMoveZ = (float)((flags & RI_MOUSE_WHEEL) ? *(SHORT*)&block.data.mouse.usButtonData : 0);
						AddEvent(e);
					}
				}
				break;
			}
		return true;
	}
	return false;
}

END_INANITY_INPUT
