#ifndef ___INANITY_INPUT_EVENT_HPP___
#define ___INANITY_INPUT_EVENT_HPP___

#include "input.hpp"

BEGIN_INANITY_INPUT

/// Структура, обозначающая событие ввода.
struct Event
{
	/// Устройство, которое прислало событие.
	enum Device
	{
		deviceKeyboard,
		deviceMouse
	} device;

	/// Данные клавиатуры.
	struct Keyboard
	{
		/// Тип события.
		enum Type
		{
			typeKeyDown,
			typeKeyUp,
			typeKeyPress
		} type;
		/// Номер клавиши.
		int key;
	};

	/// Данные мыши.
	struct Mouse
	{
		/// Тип события
		enum Type
		{
			typeButtonDown,
			typeButtonUp,
			typeMove
		} type;

		/// Кнопка мыши.
		enum Button
		{
			buttonLeft,
			buttonRight,
			buttonMiddle
		};

		/// Данные события
		union
		{
			/// Кнопка мыши (в случае typeButtonDown, typeButtonUp)
			Button button;
			/// смещение мыши (в случае typeMove)
			struct
			{
				float offsetX;
				float offsetY;
				float offsetZ;
			};
		};
	};
	/// Объединение, содержащее данные.
	union
	{
		Keyboard keyboard;
		Mouse mouse;
	};
};

END_INANITY_INPUT

#endif
