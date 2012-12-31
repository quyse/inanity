#ifndef ___INANITY_GRAPHICS_SHADERS_INTERPOLANT_IPP___
#define ___INANITY_GRAPHICS_SHADERS_INTERPOLANT_IPP___

#include "Interpolant.hpp"

BEGIN_INANITY_SHADERS

template <typename ValueType>
Interpolant<ValueType>::Interpolant(Semantic semantic)
: LValue<ValueType>(NEW(TransitionalNode(GetDataType<ValueType>(), TransitionalNode::transitionalTypeTransformed, semantic)))
{}

END_INANITY_SHADERS

#endif
