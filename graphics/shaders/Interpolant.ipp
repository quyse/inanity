#ifndef ___INANITY_GRAPHICS_SHADERS_INTERPOLANT_IPP___
#define ___INANITY_GRAPHICS_SHADERS_INTERPOLANT_IPP___

#include "Interpolant.hpp"
#include "TransitionalNode.hpp"

BEGIN_INANITY_SHADERS

template <typename ValueType>
Interpolant<ValueType>::Interpolant(Semantic semantic)
: LValue<ValueType>(NEW(TransitionalNode(GetDataType<ValueType>(), TransitionalNode::transitionalTypeTransformed, semantic)))
{}

template <typename ValueType>
Value<ValueType> Interpolant<ValueType>::operator=(Value<ValueType> a)
{
	return LValue<ValueType>::operator=(a);
}

END_INANITY_SHADERS

#endif
