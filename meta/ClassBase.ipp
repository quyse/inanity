#ifndef ___INANITY_SCRIPT_V8_CLASS_BASE_IPP
#define ___INANITY_SCRIPT_V8_CLASS_BASE_IPP

#include "ClassBase.hpp"

BEGIN_INANITY_META

template <typename Traits>
ClassBase<Traits>::ClassBase(const char* name, const char* fullName)
: name(name), fullName(fullName) {}

template <typename Traits>
const char* ClassBase<Traits>::GetName() const
{
	return name;
}

template <typename Traits>
const char* ClassBase<Traits>::GetFullName() const
{
	return fullName;
}

template <typename Traits>
void ClassBase<Traits>::SetParent(ParentClassBase* parent)
{
	this->parent = parent;
}

template <typename Traits>
typename ClassBase<Traits>::ParentClassBase* ClassBase<Traits>::GetParent() const
{
	return parent;
}

template <typename Traits>
void ClassBase<Traits>::SetConstructor(ConstructorBase* constructor)
{
	this->constructor = constructor;
}

template <typename Traits>
void ClassBase<Traits>::AddMethod(MethodBase* method)
{
	methods.push_back(method);
}

template <typename Traits>
void ClassBase<Traits>::AddStaticMethod(FunctionBase* staticMethod)
{
	staticMethods.push_back(staticMethod);
}

template <typename Traits>
typename ClassBase<Traits>::ConstructorBase* ClassBase<Traits>::GetConstructor() const
{
	return constructor;
}

template <typename Traits>
const typename ClassBase<Traits>::Methods& ClassBase<Traits>::GetMethods() const
{
	return methods;
}

template <typename Traits>
const typename ClassBase<Traits>::StaticMethods& ClassBase<Traits>::GetStaticMethods() const
{
	return staticMethods;
}

template <typename Traits>
bool ClassBase<Traits>::CanCastTo(ClassBase* targetClass) const
{
	for(const ClassBase* cls = this; cls; cls = cls->parent)
		if(cls == targetClass)
			return true;
	return false;
}

END_INANITY_META

#endif
