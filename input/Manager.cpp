#include "Manager.hpp"
#include "Frame.hpp"
#include "Controller.hpp"
#include "../CriticalCode.hpp"
#include "../Exception.hpp"
#include <algorithm>

BEGIN_INANITY_INPUT

Manager::Manager()
: textInputEnabled(false), releaseButtonsOnUpdate(false)
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

	// add release events to internal frame if needed
	if(releaseButtonsOnUpdate)
	{
		releaseButtonsOnUpdate = false;

		const State& state = internalFrame->GetCurrentState();
		for(size_t i = 0; i < sizeof(state.keyboard) / sizeof(state.keyboard[0]); ++i)
			if(state.keyboard[i])
			{
				Event e;
				e.device = Event::deviceKeyboard;
				e.keyboard.type = Event::Keyboard::typeKeyUp;
				e.keyboard.key = Key(i);
				internalFrame->AddEvent(e);
			}
		for(size_t i = 0; i < sizeof(state.mouseButtons) / sizeof(state.mouseButtons[0]); ++i)
			if(state.mouseButtons[i])
			{
				Event e;
				e.device = Event::deviceMouse;
				e.mouse.type = Event::Mouse::typeButtonUp;
				e.mouse.button = Event::Mouse::Button(i);
				internalFrame->AddEvent(e);
			}
	}
}

void Manager::ReleaseButtonsOnUpdate()
{
	CriticalCode criticalCode(criticalSection);

	releaseButtonsOnUpdate = true;
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

ptr<Controller> Manager::TryGetController(uint64_t controllerId)
{
	return nullptr;
}

END_INANITY_INPUT
