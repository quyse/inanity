#ifndef ___INANITY_INPUT_STATE_HPP___
#define ___INANITY_INPUT_STATE_HPP___

#include "input.hpp"

BEGIN_INANITY_INPUT

/// Структура, хранящая текущее состояние устройств ввода.
struct State
{
	/// Состояние клавиш клавиатуры.
	unsigned char keyboard[256];
	/// Нажатость кнопок мыши.
	bool mouseButtons[3];
	/// Координаты мыши.
	float mouseX, mouseY, mouseZ;
};

END_INANITY_INPUT

#endif
