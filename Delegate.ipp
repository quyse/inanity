#ifndef ___INANITY_DELEGATE_IPP___
#define ___INANITY_DELEGATE_IPP___

#include "Delegate.hpp"

BEGIN_INANITY

template <typename T, typename Class>
Delegate<T, Class>::Delegate(ptr<Class> object, Method method)
: object(object), method(method) {}

template <typename T, typename Class>
void Delegate<T, Class>::OnEvent(T data)
{
	(object->*method)(data);
}

END_INANITY

#endif
