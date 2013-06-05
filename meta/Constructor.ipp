#ifndef ___INANITY_META_CONSTRUCTOR_IPP___
#define ___INANITY_META_CONSTRUCTOR_IPP___

#include "Constructor.hpp"
#include "Callable.hpp"
#include "of.hpp"

BEGIN_INANITY_META

template <typename ConstructorType>
const typename Constructor<ConstructorType>::ExtensionType& Constructor<ConstructorType>::GetExtension() const
{
	return extension;
}

END_INANITY_META

#endif
