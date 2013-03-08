#ifndef ___INANITY_GRAPHICS_SHADERS_INTERPOLANT_IPP___
#define ___INANITY_GRAPHICS_SHADERS_INTERPOLANT_IPP___

#include "Interpolant.hpp"
#include "TransformedNode.hpp"

BEGIN_INANITY_SHADERS

template <typename ValueType>
Interpolant<ValueType>::Interpolant(int semantic)
: LValue<ValueType>(NEW(TransformedNode(GetDataType<ValueType>(), semantic)))
{}

template <typename ValueType>
Value<ValueType> Interpolant<ValueType>::operator=(Value<ValueType> a)
{
	return LValue<ValueType>::operator=(a);
}

END_INANITY_SHADERS

#endif
