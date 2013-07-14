#include "X11Manager.hpp"
#include "Frame.hpp"

BEGIN_INANITY_INPUT

X11Manager::X11Manager() : mouseX(0), mouseY(0) {}

static bool ConvertButton(unsigned button, Event::Mouse::Button& outButton)
{
	switch(button)
	{
	case Button1: outButton = Event::Mouse::buttonLeft; break;
	case Button2: outButton = Event::Mouse::buttonRight; break;
	case Button3: outButton = Event::Mouse::buttonMiddle; break;
	default: return false;
	}
	return true;
}

bool X11Manager::Process(const XEvent& event)
{
	Event e;

	switch(event.type)
	{
	case KeyPress:
		e.device = Event::deviceKeyboard;
		e.keyboard.type = Event::Keyboard::typeKeyDown;
		e.keyboard.key = event.xkey.keycode;
		break;
	case KeyRelease:
		e.device = Event::deviceKeyboard;
		e.keyboard.type = Event::Keyboard::typeKeyUp;
		e.keyboard.key = event.xkey.keycode;
		break;
	case ButtonPress:
		e.device = Event::deviceMouse;
		e.mouse.type = Event::Mouse::typeButtonDown;
		if(!ConvertButton(event.xbutton.button, e.mouse.button))
			return false;
		break;
	case ButtonRelease:
		e.device = Event::deviceMouse;
		e.mouse.type = Event::Mouse::typeButtonUp;
		if(!ConvertButton(event.xbutton.button, e.mouse.button))
			return false;
		break;
	case MotionNotify:
		e.device = Event::deviceMouse;
		e.mouse.type = Event::Mouse::typeMove;
		{
			const State& state = internalFrame->GetCurrentState();
			e.mouse.offsetX = event.xmotion.x - mouseX;
			e.mouse.offsetY = event.xmotion.y - mouseY;
			e.mouse.offsetZ = 0;
			mouseX = event.xmotion.x;
			mouseY = event.xmotion.y;
		}
		break;
	default:
		return false;
	}

	AddEvent(e);

	return true;
}

END_INANITY_INPUT
