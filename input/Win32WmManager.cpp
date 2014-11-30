#include "Win32WmManager.hpp"
#include "Frame.hpp"

BEGIN_INANITY_INPUT

bool Win32WmManager::ProcessWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(Win32Manager::ProcessWindowMessage(msg, wParam, lParam))
		return true;

	switch(msg)
	{
	case WM_KEYDOWN:
		{
			Event e;
			e.device = Event::deviceKeyboard;
			e.keyboard.type = Event::Keyboard::typeKeyDown;
			e.keyboard.key = ConvertKey((USHORT)wParam);
			AddEvent(e);
		}
		break;
	case WM_KEYUP:
		{
			Event e;
			e.device = Event::deviceKeyboard;
			e.keyboard.type = Event::Keyboard::typeKeyUp;
			e.keyboard.key = ConvertKey((USHORT)wParam);
			AddEvent(e);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			Event e;
			e.device = Event::deviceMouse;
			e.mouse.type = Event::Mouse::typeButtonDown;
			e.mouse.button = Event::Mouse::buttonLeft;
			AddEvent(e);
		}
		break;
	case WM_LBUTTONUP:
		{
			Event e;
			e.device = Event::deviceMouse;
			e.mouse.type = Event::Mouse::typeButtonUp;
			e.mouse.button = Event::Mouse::buttonLeft;
			AddEvent(e);
		}
		break;
	case WM_RBUTTONDOWN:
		{
			Event e;
			e.device = Event::deviceMouse;
			e.mouse.type = Event::Mouse::typeButtonDown;
			e.mouse.button = Event::Mouse::buttonRight;
			AddEvent(e);
		}
		break;
	case WM_RBUTTONUP:
		{
			Event e;
			e.device = Event::deviceMouse;
			e.mouse.type = Event::Mouse::typeButtonUp;
			e.mouse.button = Event::Mouse::buttonRight;
			AddEvent(e);
		}
		break;

	case WM_MBUTTONDOWN:
		{
			Event e;
			e.device = Event::deviceMouse;
			e.mouse.type = Event::Mouse::typeButtonDown;
			e.mouse.button = Event::Mouse::buttonMiddle;
			AddEvent(e);
		}
		break;
	case WM_MBUTTONUP:
		{
			Event e;
			e.device = Event::deviceMouse;
			e.mouse.type = Event::Mouse::typeButtonUp;
			e.mouse.button = Event::Mouse::buttonMiddle;
			AddEvent(e);
		}
		break;
	case WM_MOUSEWHEEL:
		{
			Event e;
			e.device = Event::deviceMouse;
			e.mouse.type = Event::Mouse::typeRawMove;
			e.mouse.rawMoveX = 0;
			e.mouse.rawMoveY = 0;
			e.mouse.rawMoveZ = HIWORD(wParam);
			AddEvent(e);
		}
		break;
	default:
		return false;
	}
	return true;
}

END_INANITY_INPUT
