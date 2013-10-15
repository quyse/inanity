#ifndef ___INANITY_INPUT_STATE_HPP___
#define ___INANITY_INPUT_STATE_HPP___

#include "input.hpp"

BEGIN_INANITY_INPUT

/// Структура, хранящая текущее состояние устройств ввода.
struct State
{
	/// Keyboard keys state.
	unsigned char keyboard[256];
	/// Mouse buttons state.
	bool mouseButtons[3];
	/// Mouse cursor position.
	/** Mouse cursor as moved by OS. */
	int cursorX, cursorY;
};

END_INANITY_INPUT

#endif
