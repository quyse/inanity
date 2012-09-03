#ifndef ___INANITY_EVENT_HANDLER_HPP___
#define ___INANITY_EVENT_HANDLER_HPP___

#include "Object.hpp"

BEGIN_INANITY

/// Абстрактный класс обработчика событий.
/** Предназначен для обратной связи различных классов. */
template <typename T>
class Handler : public Object
{
protected:
	/// Обработать событие.
	/** Собственно, метод, который нужно перегружать в производном классе. */
	virtual void OnEvent(T data) = 0;

public:
	/// Сообщить о наступлении события.
	void Fire(T data)
	{
		OnEvent(data);
	}
};

/// Специальная версия без параметров.
class VoidHandler : public Object
{
protected:
	virtual void OnEvent() = 0;

public:
	void Fire()
	{
		OnEvent();
	}
};

END_INANITY

#endif
