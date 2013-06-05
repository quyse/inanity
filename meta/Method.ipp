#ifndef ___INANITY_META_METHOD_IPP___
#define ___INANITY_META_METHOD_IPP___

#include "Method.hpp"
#include "Callable.hpp"
#include "of.hpp"

BEGIN_INANITY_META

template <typename MethodType, MethodType method>
Method<MethodType, method>::Method(const char* name) : MethodBase(name)
{
	// register the method in the class
	OfClass<typename Callable<MethodType>::ClassType>::meta.AddMethod(this);
}

template <typename MethodType, MethodType method>
const typename Method<MethodType, method>::ExtensionType& Method<MethodType, method>::GetExtension() const
{
	return extension;
}

END_INANITY_META

#endif
