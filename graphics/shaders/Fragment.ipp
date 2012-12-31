#ifndef ___INANITY_GRAPHICS_SHADERS_FRAGMENT_IPP___
#define ___INANITY_GRAPHICS_SHADERS_FRAGMENT_IPP___

#include "Fragment.hpp"

BEGIN_INANITY_SHADERS

template <typename ValueType>
Fragment<ValueType>::Fragment(Semantic semantic)
: LValue<ValueType>(NEW(TransitionalNode(GetDataType<ValueType>(), TransitionalNode::transitionalTypeRasterized, semantic)))
{}

END_INANITY_SHADERS

#endif
