#ifndef ___INANITY_META_FUNCTION_IPP___
#define ___INANITY_META_FUNCTION_IPP___

#include "Function.hpp"

BEGIN_INANITY_META

template <typename FunctionType, FunctionType function>
Function<FunctionType, function>::Function(const char* name) : FunctionBase(name) {}

template <typename FunctionType, FunctionType function>
const typename Function<FunctionType, function>::ExtensionType& Function<FunctionType, function>::GetExtension() const
{
	return extension;
}

END_INANITY_META

#endif
