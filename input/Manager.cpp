#include "Manager.hpp"
#include "Frame.hpp"
#include "../CriticalCode.hpp"
#include "../Exception.hpp"
#include <algorithm>

Manager::Manager(HWND hWnd)
: hWnd(hWnd)
{
	currentFrame = NEW(Frame);
	internalFrame = NEW(Frame);
}

bool Manager::ProcessWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam)
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

void Manager::AddEvent(const Event& e)
{
	CriticalCode criticalCode(criticalSection);

	internalFrame->AddEvent(e);
}

void Manager::Update()
{
	CriticalCode criticalCode(criticalSection);

	//теперь текущим кадром становится внутренний
	std::swap(currentFrame, internalFrame);
	//у нового внутреннего кадра нужно сделать корректное состояние
	//оно должно быть равно состоянию старого внутреннего кадра
	//как если бы мы выполнили бы все события
	currentFrame->CopyTo(internalFrame);
	//перемотать скопированные события
	internalFrame->ForwardEvents();
	//сбросить данные внутреннего кадра
	internalFrame->Reset();
}

ptr<Frame> Manager::GetCurrentFrame()
{
	return currentFrame;
}

void Manager::AcquireDevices()
{
}

void Manager::UnacquireDevices()
{
}
