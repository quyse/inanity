#ifndef ___INANITY_META_CLASS_IPP___
#define ___INANITY_META_CLASS_IPP___

#include "Class.hpp"

BEGIN_INANITY_META

template <typename ClassType>
Class<ClassType>::Class(const char* name, const char* fullName, ClassBase* parent)
: ClassBase(name, fullName, parent)
{
	ClassInitialize<ClassType>(*this);
}

template <typename ClassType>
const typename Class<ClassType>::ExtensionType& Class<ClassType>::GetExtension() const
{
	return extension;
}

END_INANITY_META

#endif
