#ifndef ___INANITY_GUI_NOTIFICATIONS_HPP___
#define ___INANITY_GUI_NOTIFICATIONS_HPP___

#include "gui.hpp"

BEGIN_INANITY_GUI

/// Структура с перечислением уведомлений, которые элементы могут присылать.
struct Notifications
{
	enum
	{
		Click,
		TextChanged
	};
};

END_INANITY_GUI

#endif
