#ifndef ___INANITY_EVENT_HANDLER_HPP___
#define ___INANITY_EVENT_HANDLER_HPP___

#include "Object.hpp"

BEGIN_INANITY

/// Абстрактный класс обработчика событий.
/** Предназначен для обратной связи различных классов. */
template <typename T>
class EventHandler : public Object
{
protected:
	/// Обработать событие.
	/** Собственно, метод, который нужно перегружать в производном классе. */
	virtual void OnEvent(T data) = 0;

public:
	EventHandler() {}

	/// Сообщить о наступлении события.
	void Fire(T data)
	{
		OnEvent(data);
	}

	template <typename Class>
	class Delegate : public EventHandler<T>
	{
	public:
		typedef void (Class::*Method)(T data);

	private:
		ptr<Class> object;
		Method method;

	public:
		Delegate(ptr<Class> object, Method method) : object(object), method(method) {}

		void OnEvent(T data)
		{
			(object->*method)(data);
		}
	};

	template <typename Class>
	static ptr<EventHandler<T> > Create(ptr<Class> object, typename Delegate<Class>::Method method)
	{
		return NEW(Delegate<Class>(object, method));
	}

	template <typename Class>
	static ptr<EventHandler<T> > Create(Class* object, typename Delegate<Class>::Method method)
	{
		return Create<Class>(ptr<Class>(object), method);
	}
};

END_INANITY

#endif
