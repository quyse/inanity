#ifndef ___INANITY_GUI_NOTIFICATOR_HPP___
#define ___INANITY_GUI_NOTIFICATOR_HPP___

#include "gui.hpp"

BEGIN_INANITY_GUI

class Element;

/// Абстрактный класс, принимающий GUI-уведомления.
class Notificator : public Object
{
public:
	/// Передать уведомление.
	virtual void Notify(ptr<Element> element, int code) = 0;
};

END_INANITY_GUI

#endif
