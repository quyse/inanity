#include "Manager.hpp"
#include "Frame.hpp"
#include "Controller.hpp"
#include "../CriticalCode.hpp"
#include "../Exception.hpp"
#include <algorithm>

BEGIN_INANITY_INPUT

Manager::Manager()
: textInputEnabled(false)
{
	currentFrame = NEW(Frame);
	internalFrame = NEW(Frame);
}

void Manager::AddEvent(const Event& e)
{
	CriticalCode criticalCode(criticalSection);

	// skip text input events if text input is disabled
	if(e.device == Event::deviceKeyboard && e.keyboard.type == Event::Keyboard::typeCharacter && !textInputEnabled) return;

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

void Manager::StartTextInput()
{
	CriticalCode criticalCode(criticalSection);

	textInputEnabled = true;
}

void Manager::StopTextInput()
{
	CriticalCode criticalCode(criticalSection);

	textInputEnabled = false;
}

bool Manager::IsTextInput() const
{
	return textInputEnabled;
}

ptr<Controller> Manager::TryGetController(int controllerId)
{
	return nullptr;
}

END_INANITY_INPUT
