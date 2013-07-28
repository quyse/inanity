#ifndef ___INANITY_INPUT_EVENT_HPP___
#define ___INANITY_INPUT_EVENT_HPP___

#include "Key.hpp"
#include <iostream>

BEGIN_INANITY_INPUT

struct Event
{
	enum Device
	{
		deviceKeyboard,
		deviceMouse
	} device;

	struct Keyboard
	{
		enum Type
		{
			typeKeyDown,
			typeKeyUp,
			typeCharacter
		} type;

		union
		{
			// in case type == typeKey{Down,Up}
			Key key;
			// in case type == typeCharacter
			wchar_t character;
		};
	};

	struct Mouse
	{
		enum Type
		{
			typeButtonDown,
			typeButtonUp,
			typeMove
		} type;

		enum Button
		{
			buttonLeft,
			buttonRight,
			buttonMiddle
		};

		union
		{
			// in case type == typeButton{Down,Up}
			Button button;
			// in case type == typeMove
			struct
			{
				float offsetX;
				float offsetY;
				float offsetZ;
			};
		};
	};

	union
	{
		// in case device == deviceKeyboard
		Keyboard keyboard;
		// in case device == deviceMouse
		Mouse mouse;
	};

	// debug output into std::ostream
	friend std::ostream& operator<<(std::ostream& stream, const Event& event);
};

END_INANITY_INPUT

#endif
