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

	// switch frames
	std::swap(currentFrame, internalFrame);
	// old internal frame (new current frame)
	// should be copied to new internal frame,
	// and state is rewinded forward
	currentFrame->CopyTo(internalFrame);
	internalFrame->ForwardEvents();
	internalFrame->Reset();
}

ptr<Frame> Manager::GetCurrentFrame()
{
	return currentFrame;
}

END_INANITY_INPUT
