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
	Delegate(ptr<Class> object, Method method);

	void OnEvent(T data);
};

END_INANITY

#endif
