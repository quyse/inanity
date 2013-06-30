#include "Win32Manager.hpp"
#include "Frame.hpp"

BEGIN_INANITY_INPUT

Win32Manager::Win32Manager(HWND hWnd) : hWnd(hWnd) {}

bool Win32Manager::ProcessWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	//по умолчанию выполняется обработка символов
	if(msg == WM_CHAR)
	{
		Event e;
		e.device = Event::deviceKeyboard;
		e.keyboard.type = Event::Keyboard::typeKeyPress;
		e.keyboard.key = (int)wParam;
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
