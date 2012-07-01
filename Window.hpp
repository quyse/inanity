#ifndef ___INANITY_WINDOW_HPP___
#define ___INANITY_WINDOW_HPP___

#include "Object.hpp"

BEGIN_INANITY

/// Абстрактный класс окна.
class Window : public Object
{
public:
	/// Указать заголовок окна.
	virtual void SetTitle(const String& title) = 0;
};

END_INANITY

#endif
