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
		}
		stream << ' ';
		switch(event.mouse.type)
		{
			case Event::Mouse::typeButtonDown:
			case Event::Mouse::typeButtonUp:
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
				stream << event.mouse.cursorX << ' ' << event.mouse.cursorY;
				break;
		}
		break;
	}

	return stream;
}

END_INANITY_INPUT
