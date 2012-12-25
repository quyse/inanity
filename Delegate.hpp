#ifndef ___INANITY_DELEGATE_HPP___
#define ___INANITY_DELEGATE_HPP___

#include "Handler.hpp"

BEGIN_INANITY

/// Класс делегата - обработчика, который просто вызывает метод у объекта.
template <typename T, typename Class>
class Delegate : public Handler<T>
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

/// Класс делегата без параметра.
template <typename T>
class VoidDelegate : public VoidHandler
{
public:
	typedef void (Class::*Method)();

private:
	ptr<Class> object;
	Method method;

public:
	Delegate(ptr<Class> object, Method method) : object(object), method(method) {}

	void OnEvent()
	{
		(object->*method)();
	}
};

END_INANITY

#endif
