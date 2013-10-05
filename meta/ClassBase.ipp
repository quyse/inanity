#ifndef ___INANITY_SCRIPT_V8_CLASS_BASE_IPP
#define ___INANITY_SCRIPT_V8_CLASS_BASE_IPP

#include "ClassBase.hpp"

BEGIN_INANITY_META

template <typename ConstructorBase, typename FunctionBase, typename MethodBase>
ClassBase<ConstructorBase, FunctionBase, MethodBase>::ClassBase(const char* name, const char* fullName)
: name(name), fullName(fullName) {}

template <typename ConstructorBase, typename FunctionBase, typename MethodBase>
const char* ClassBase<ConstructorBase, FunctionBase, MethodBase>::GetName() const
{
	return name;
}

template <typename ConstructorBase, typename FunctionBase, typename MethodBase>
const char* ClassBase<ConstructorBase, FunctionBase, MethodBase>::GetFullName() const
{
	return fullName;
}

template <typename ConstructorBase, typename FunctionBase, typename MethodBase>
void ClassBase<ConstructorBase, FunctionBase, MethodBase>::SetParent(ClassBase* parent)
{
	this->parent = parent;
}

template <typename ConstructorBase, typename FunctionBase, typename MethodBase>
ClassBase<ConstructorBase, FunctionBase, MethodBase>* ClassBase<ConstructorBase, FunctionBase, MethodBase>::GetParent() const
{
	return parent;
}

template <typename ConstructorBase, typename FunctionBase, typename MethodBase>
void ClassBase<ConstructorBase, FunctionBase, MethodBase>::SetConstructor(ConstructorBase* constructor)
{
	this->constructor = constructor;
}

template <typename ConstructorBase, typename FunctionBase, typename MethodBase>
void ClassBase<ConstructorBase, FunctionBase, MethodBase>::AddMethod(MethodBase* method)
{
	methods.push_back(method);
}

template <typename ConstructorBase, typename FunctionBase, typename MethodBase>
void ClassBase<ConstructorBase, FunctionBase, MethodBase>::AddStaticMethod(FunctionBase* staticMethod)
{
	staticMethods.push_back(staticMethod);
}

template <typename ConstructorBase, typename FunctionBase, typename MethodBase>
ConstructorBase* ClassBase<ConstructorBase, FunctionBase, MethodBase>::GetConstructor() const
{
	return constructor;
}

template <typename ConstructorBase, typename FunctionBase, typename MethodBase>
const typename ClassBase<ConstructorBase, FunctionBase, MethodBase>::Methods& ClassBase<ConstructorBase, FunctionBase, MethodBase>::GetMethods() const
{
	return methods;
}

template <typename ConstructorBase, typename FunctionBase, typename MethodBase>
const typename ClassBase<ConstructorBase, FunctionBase, MethodBase>::StaticMethods& ClassBase<ConstructorBase, FunctionBase, MethodBase>::GetStaticMethods() const
{
	return staticMethods;
}

END_INANITY_META

#endif
