#include "Frame.hpp"
#include "../Exception.hpp"
#include <cstring>

BEGIN_INANITY_INPUT

Frame::Frame()
{
	Reset();
	memset(&state, 0, sizeof(state));
}

const Event& Frame::GetCurrentEvent() const
{
	if(nextEvent > 0 && nextEvent <= events.size())
		return events[nextEvent - 1];
	THROW("There is no current event");
}

const State& Frame::GetCurrentState() const
{
	return state;
}

bool Frame::NextEvent()
{
	// if next event is present
	if(nextEvent < events.size())
	{
		// process next event in state
		const Event& e = events[nextEvent];

		switch(e.device)
		{
		case Event::deviceKeyboard:
			switch(e.keyboard.type)
			{
			case Event::Keyboard::typeKeyDown:
				state.keyboard[e.keyboard.key] = 1;
				break;
			case Event::Keyboard::typeKeyUp:
				state.keyboard[e.keyboard.key] = 0;
				break;
			}
			break;
		case Event::deviceMouse:
			switch(e.mouse.type)
			{
			case Event::Mouse::typeButtonDown:
				state.mouseButtons[e.mouse.button] = true;
				break;
			case Event::Mouse::typeButtonUp:
				state.mouseButtons[e.mouse.button] = false;
				break;
			case Event::Mouse::typeRawMove:
				// there is no state for raw move
				break;
			case Event::Mouse::typeCursorMove:
				state.cursorX += e.mouse.cursorMoveX;
				state.cursorY += e.mouse.cursorMoveY;
				break;
			}
			break;
		}
	}

	// make the next event current
	return nextEvent++ < events.size();
}

void Frame::ForwardEvents()
{
	while(NextEvent());
}

void Frame::Reset()
{
	events.clear();
	nextEvent = 0;
}

void Frame::AddEvent(const Event& e)
{
	events.push_back(e);
}

void Frame::CopyTo(ptr<Frame> frame)
{
	frame->events = events;
	frame->nextEvent = nextEvent;
	frame->state = state;
}

END_INANITY_INPUT
