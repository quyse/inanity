#include "Win32Manager.hpp"
#include "Frame.hpp"

BEGIN_INANITY_INPUT

Key Win32Manager::ConvertKey(USHORT key)
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
	K(DELETE, Delete);
	K(HOME, Home);
	K(LEFT, Left);
	K(UP, Up);
	K(RIGHT, Right);
	K(DOWN, Down);
	K(PRIOR, PageUp);
	K(NEXT, PageDown);
	K(END, End);
	K(NUMLOCK, NumLock);
	K(NUMPAD0, KeyPad0);
	K(NUMPAD1, KeyPad1);
	K(NUMPAD2, KeyPad2);
	K(NUMPAD3, KeyPad3);
	K(NUMPAD4, KeyPad4);
	K(NUMPAD5, KeyPad5);
	K(NUMPAD6, KeyPad6);
	K(NUMPAD7, KeyPad7);
	K(NUMPAD8, KeyPad8);
	K(NUMPAD9, KeyPad9);
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
	K(LSHIFT, ShiftL);
	K(RSHIFT, ShiftR);
	K(LCONTROL, ControlL);
	K(RCONTROL, ControlR);
	K(CAPITAL, CapsLock);
	K(LMENU, AltL);
	K(RMENU, AltR);
	K(LWIN, SuperL);
	K(RWIN, SuperR);
	K(SPACE, Space);
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
#undef K
#undef KN
#undef KA
	default:
		return Keys::_Unknown;
	}
}

Win32Manager::Win32Manager(HWND hWnd) : hWnd(hWnd) {}

bool Win32Manager::ProcessWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	//по умолчанию выполняется обработка символов
	if(msg == WM_CHAR)
	{
		Event e;
		e.device = Event::deviceKeyboard;
		e.keyboard.type = Event::Keyboard::typeCharacter;
		e.keyboard.character = (wchar_t)wParam;
		AddEvent(e);
		return true;
	}
	return false;
}

void Win32Manager::AcquireDevices()
{
}

void Win32Manager::UnacquireDevices()
{
}

END_INANITY_INPUT
