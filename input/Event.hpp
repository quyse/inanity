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
		deviceMouse,
		deviceController
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
			typeCursorMove,
			typeDoubleClick
		} type;

		enum Button
		{
			buttonLeft,
			buttonRight,
			buttonMiddle
		};

		union
		{
			// in case type == typeButton{Down,Up,DoubleClick}
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
				//*** Relative mouse wheel movement.
				int cursorZ;
			};
		};
	};

	struct Controller
	{
		enum Type
		{
			typeDeviceAdded,
			typeDeviceRemoved,
			typeButtonDown,
			typeButtonUp,
			typeAxisMotion
		} type;

		enum Button
		{
			buttonA,
			buttonB,
			buttonX,
			buttonY,
			buttonBack,
			buttonGuide,
			buttonStart,
			buttonLeftStick,
			buttonRightStick,
			buttonLeftShoulder,
			buttonRightShoulder,
			buttonDPadUp,
			buttonDPadDown,
			buttonDPadLeft,
			buttonDPadRight
		};

		enum Axis
		{
			axisLeftX,
			axisLeftY,
			axisRightX,
			axisRightY,
			axisTriggerLeft,
			axisTriggerRight
		};

		/// Device id (for all event types).
		int device;

		union
		{
			// in case type == typeButton{Down,Up}
			Button button;
			// in case type == typeAxisMotion
			struct
			{
				Axis axis;
				int axisValue;
			};
		};
	};

	union
	{
		// in case device == deviceKeyboard
		Keyboard keyboard;
		// in case device == deviceMouse
		Mouse mouse;
		// in case device == deviceController
		Controller controller;
	};

	// debug output into std::ostream
	friend std::ostream& operator<<(std::ostream& stream, const Event& event);
};

END_INANITY_INPUT

#endif
