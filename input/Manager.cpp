#include "Manager.hpp"
#include "Frame.hpp"
#include "../CriticalCode.hpp"
#include "../Exception.hpp"
#include <algorithm>

BEGIN_INANITY_INPUT

Manager::Manager()
{
	currentFrame = NEW(Frame);
	internalFrame = NEW(Frame);
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

END_INANITY_INPUT
