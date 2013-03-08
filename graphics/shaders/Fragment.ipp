#ifndef ___INANITY_GRAPHICS_SHADERS_FRAGMENT_IPP___
#define ___INANITY_GRAPHICS_SHADERS_FRAGMENT_IPP___

#include "Fragment.hpp"
#include "RasterizedNode.hpp"

BEGIN_INANITY_SHADERS

template <typename ValueType>
Fragment<ValueType>::Fragment(int semantic)
: LValue<ValueType>(NEW(RasterizedNode(GetDataType<ValueType>(), semantic)))
{}

template <typename ValueType>
Value<ValueType> Fragment<ValueType>::operator=(Value<ValueType> a)
{
	return LValue<ValueType>::operator=(a);
}

END_INANITY_SHADERS

#endif
