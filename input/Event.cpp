#include "Event.hpp"
#include "../Strings.hpp"

BEGIN_INANITY_INPUT

std::ostream& operator<<(std::ostream& stream, const Event& event)
{
	switch(event.device)
	{
	case Event::deviceKeyboard:
		switch(event.keyboard.type)
		{
		case Event::Keyboard::typeKeyDown:
			stream << "KEYDOWN " << event.keyboard.key;
			break;
		case Event::Keyboard::typeKeyUp:
			stream << "KEYUP " << event.keyboard.key;
			break;
		case Event::Keyboard::typeCharacter:
			{
				wchar_t s[2] = { event.keyboard.character, 0 };
				stream << "KEYPRESS " << Strings::Unicode2UTF8(s);
			}
			break;
		}
		break;
	case Event::deviceMouse:
		switch(event.mouse.type)
		{
			case Event::Mouse::typeButtonDown:
				stream << "MOUSEDOWN";
				break;
			case Event::Mouse::typeButtonUp:
				stream << "MOUSEUP";
				break;
			case Event::Mouse::typeRawMove:
				stream << "MOUSERAWMOVE";
				break;
			case Event::Mouse::typeCursorMove:
				stream << "MOUSECURSORMOVE";
				break;
			case Event::Mouse::typeDoubleClick:
				stream << "MOUSEDOUBLECLICK";
				break;
		}
		stream << ' ';
		switch(event.mouse.type)
		{
			case Event::Mouse::typeButtonDown:
			case Event::Mouse::typeButtonUp:
			case Event::Mouse::typeDoubleClick:
				switch(event.mouse.button)
				{
				case Event::Mouse::buttonLeft:
					stream << "LEFT";
					break;
				case Event::Mouse::buttonRight:
					stream << "RIGHT";
					break;
				case Event::Mouse::buttonMiddle:
					stream << "MIDDLE";
					break;
				}
				break;
			case Event::Mouse::typeRawMove:
				stream << event.mouse.rawMoveX << ' ' << event.mouse.rawMoveY << ' ' << event.mouse.rawMoveZ;
				break;
			case Event::Mouse::typeCursorMove:
				stream << event.mouse.cursorX << ' ' << event.mouse.cursorY << ' ' << event.mouse.cursorZ;
				break;
		}
		break;
	case Event::deviceController:
		switch(event.controller.type)
		{
		case Event::Controller::typeDeviceAdded:
			stream << "CONTROLLERADDED";
			break;
		case Event::Controller::typeDeviceRemoved:
			stream << "CONTROLLERREMOVED";
			break;
		case Event::Controller::typeButtonDown:
			stream << "CONTROLLERBUTTONDOWN";
			break;
		case Event::Controller::typeButtonUp:
			stream << "CONTROLLERBUTTONUP";
			break;
		case Event::Controller::typeAxisMotion:
			stream << "CONTROLLERAXISMOTION";
			break;
		}
		stream << " D" << event.controller.device;
		switch(event.controller.type)
		{
		case Event::Controller::typeDeviceAdded:
		case Event::Controller::typeDeviceRemoved:
			break;
		case Event::Controller::typeButtonDown:
		case Event::Controller::typeButtonUp:
			stream << ' ';
			switch(event.controller.button)
			{
			case Event::Controller::buttonA:             stream << "A"; break;
			case Event::Controller::buttonB:             stream << "B"; break;
			case Event::Controller::buttonX:             stream << "X"; break;
			case Event::Controller::buttonY:             stream << "Y"; break;
			case Event::Controller::buttonBack:          stream << "BACK"; break;
			case Event::Controller::buttonGuide:         stream << "GUIDE"; break;
			case Event::Controller::buttonStart:         stream << "START"; break;
			case Event::Controller::buttonLeftStick:     stream << "LEFTSTICK"; break;
			case Event::Controller::buttonRightStick:    stream << "RIGHTSTICK"; break;
			case Event::Controller::buttonLeftShoulder:  stream << "LEFTSHOULDER"; break;
			case Event::Controller::buttonRightShoulder: stream << "RIGHTSHOULDER"; break;
			case Event::Controller::buttonDPadUp:        stream << "DPADUP"; break;
			case Event::Controller::buttonDPadDown:      stream << "DPADDOWN"; break;
			case Event::Controller::buttonDPadLeft:      stream << "DPADLEFT"; break;
			case Event::Controller::buttonDPadRight:     stream << "DPADRIGHT"; break;
			}
			break;
		case Event::Controller::typeAxisMotion:
			stream << ' ';
			switch(event.controller.axis)
			{
			case Event::Controller::axisLeftX:        stream << "AXISLEFTX"; break;
			case Event::Controller::axisLeftY:        stream << "AXISLEFTY"; break;
			case Event::Controller::axisRightX:       stream << "AXISRIGHTX"; break;
			case Event::Controller::axisRightY:       stream << "AXISRIGHTY"; break;
			case Event::Controller::axisTriggerLeft:  stream << "AXISTRIGGERLEFT"; break;
			case Event::Controller::axisTriggerRight: stream << "AXISTRIGGERRIGHT"; break;
			}
			stream << ' ' << event.controller.axisValue;
			break;
		}
	}

	return stream;
}

END_INANITY_INPUT
