#ifndef ___INANITY_HANDLER_HPP___
#define ___INANITY_HANDLER_HPP___

#include "Object.hpp"

BEGIN_INANITY

template <typename T, typename Class>
class Delegate;

/// Абстрактный класс обработчика.
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

/// Абстрактный класс обработчика с одним параметром.
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

	template <typename Class>
	static ptr<Handler> CreateDelegate(ptr<Class> object, void (Class::*method)(T data))
	{
		return NEW(Delegate<T, Class>(object, method));
	}
};

/// Абстрактный класс обработчика с двумя параметрами.
template <typename T1, typename T2>
class Handler2 : public Object
{
protected:
	/// Обработать событие.
	/** Собственно, метод, который нужно перегружать в производном классе. */
	virtual void OnEvent(T1 data1, T2 data2) = 0;

public:
	/// Сообщить о наступлении события.
	void Fire(T1 data1, T2 data2)
	{
		OnEvent(data1, data2);
	}
};

END_INANITY

#endif
