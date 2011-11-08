#include "Frame.hpp"
#include "../Exception.hpp"

Frame::Frame()
{
	Reset();
	memset(&state, 0, sizeof(state));
}

const Event& Frame::GetCurrentEvent() const
{
	if(currentEvent < events.size())
		return events[currentEvent];
	THROW_PRIMARY_EXCEPTION("There is no current event");
}

const State& Frame::GetCurrentState() const
{
	return state;
}

bool Frame::NextEvent()
{
	//если текущее событие есть
	if(currentEvent < events.size())
	{
		//учесть текущее событие в состоянии
		const Event& e = events[currentEvent];
		//выбор по устройству
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
			case Event::Mouse::typeMove:
				state.mouseX += e.mouse.offsetX;
				state.mouseY += e.mouse.offsetY;
				state.mouseZ += e.mouse.offsetZ;
				break;
			}
			break;
		}
	}

	//перейти к следующему событию
	return ++currentEvent < events.size();
}

void Frame::ForwardEvents()
{
	while(NextEvent());
}

void Frame::Reset()
{
	events.clear();
	currentEvent = (size_t)-1;
}

void Frame::AddEvent(const Event& e)
{
	events.push_back(e);
}

void Frame::CopyTo(ptr<Frame> frame)
{
	frame->events = events;
	frame->currentEvent = currentEvent;
	frame->state = state;
}
