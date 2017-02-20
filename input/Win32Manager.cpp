#include "Win32Manager.hpp"
#include "Frame.hpp"

BEGIN_INANITY_INPUT

Key Win32Manager::ConvertKey(USHORT key, USHORT makeCode, USHORT flags)
{
	switch(key)
	{
#define K(a, b) case VK_##a: return Keys::b
#define KN(a) case '0' + a: return Keys::_##a
#define KA(a, b) case a: return Keys::b
	K(BACK, BackSpace);
	K(TAB, Tab);
	K(CLEAR, Clear);
	K(RETURN, Return);
	K(PAUSE, Pause);
	K(SCROLL, ScrollLock);
	K(ESCAPE, Escape);
	K(INSERT, Insert);
	K(DELETE, Delete);

	K(SPACE, Space);

	K(OEM_PLUS, Plus);
	K(OEM_COMMA, Comma);
	K(OEM_MINUS, Hyphen);
	K(OEM_PERIOD, Period);
	K(OEM_2, Slash);

	KN(0);
	KN(1);
	KN(2);
	KN(3);
	KN(4);
	KN(5);
	KN(6);
	KN(7);
	KN(8);
	KN(9);

	KA('A', A);
	KA('B', B);
	KA('C', C);
	KA('D', D);
	KA('E', E);
	KA('F', F);
	KA('G', G);
	KA('H', H);
	KA('I', I);
	KA('J', J);
	KA('K', K);
	KA('L', L);
	KA('M', M);
	KA('N', N);
	KA('O', O);
	KA('P', P);
	KA('Q', Q);
	KA('R', R);
	KA('S', S);
	KA('T', T);
	KA('U', U);
	KA('V', V);
	KA('W', W);
	KA('X', X);
	KA('Y', Y);
	KA('Z', Z);

	K(HOME, Home);
	K(LEFT, Left);
	K(UP, Up);
	K(RIGHT, Right);
	K(DOWN, Down);
	K(PRIOR, PageUp);
	K(NEXT, PageDown);
	K(END, End);
	K(OEM_3, Grave);

	K(NUMLOCK, NumLock);
	K(NUMPAD0, NumPad0);
	K(NUMPAD1, NumPad1);
	K(NUMPAD2, NumPad2);
	K(NUMPAD3, NumPad3);
	K(NUMPAD4, NumPad4);
	K(NUMPAD5, NumPad5);
	K(NUMPAD6, NumPad6);
	K(NUMPAD7, NumPad7);
	K(NUMPAD8, NumPad8);
	K(NUMPAD9, NumPad9);

	K(F1, F1);
	K(F2, F2);
	K(F3, F3);
	K(F4, F4);
	K(F5, F5);
	K(F6, F6);
	K(F7, F7);
	K(F8, F8);
	K(F9, F9);
	K(F10, F10);
	K(F11, F11);
	K(F12, F12);

	case VK_SHIFT: return makeCode == 0x36 ? Keys::ShiftR : Keys::ShiftL;
	K(LSHIFT, ShiftL);
	K(RSHIFT, ShiftR);
	case VK_CONTROL: return (flags & RI_KEY_E0) ? Keys::ControlR : Keys::ControlL;
	K(LCONTROL, ControlL);
	K(RCONTROL, ControlR);
	K(CAPITAL, CapsLock);
	case VK_MENU: return (flags & RI_KEY_E0) ? Keys::AltR : Keys::AltL;
	K(LMENU, AltL);
	K(RMENU, AltR);
	K(LWIN, SuperL);
	K(RWIN, SuperR);
#undef K
#undef KN
#undef KA
	default:
		return Keys::_Unknown;
	}
}

bool Win32Manager::ProcessWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	// process characters
	case WM_CHAR:
		{
			Event e;
			e.device = Event::deviceKeyboard;
			e.keyboard.type = Event::Keyboard::typeCharacter;
			e.keyboard.character = (wchar_t)wParam;
			AddEvent(e);
		}
		return true;
	// process cursor mouse move
	case WM_MOUSEMOVE:
		{
			Event e;
			e.device = Event::deviceMouse;
			e.mouse.type = Event::Mouse::typeCursorMove;
			e.mouse.cursorX = LOWORD(lParam);
			e.mouse.cursorY = HIWORD(lParam);
			AddEvent(e);
		}
		return true;
	// process double clicks
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
		{
			Event e;
			e.device = Event::deviceMouse;
			e.mouse.type = Event::Mouse::typeDoubleClick;
			switch(msg)
			{
			case WM_LBUTTONDBLCLK: e.mouse.button = Event::Mouse::buttonLeft; break;
			case WM_RBUTTONDBLCLK: e.mouse.button = Event::Mouse::buttonRight; break;
			case WM_MBUTTONDBLCLK: e.mouse.button = Event::Mouse::buttonMiddle; break;
			}
			AddEvent(e);
		}
		return true;
	}
	return false;
}

END_INANITY_INPUT
