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
			typeRawMove,
			typeCursorMove
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
			// in case type == typeRawMove
			struct
			{
				//*** Raw relative moving.
				/** In "device" arbitrary units, no kind of acceleration applied. */
				float rawMoveX;
				float rawMoveY;
				float rawMoveZ;
			};
			// in case type == typeCursorMove
			struct
			{
				//*** Absolute cursor position (in window coordinates).
				int cursorX;
				int cursorY;
				//*** OS-dependent relative cursor moving.
				/** In pixels, cropped by window, mouse ballistics applied. */
				int cursorMoveX;
				int cursorMoveY;
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
