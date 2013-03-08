#ifndef ___INANITY_GRAPHICS_SHADERS_ATTRIBUTE_IPP___
#define ___INANITY_GRAPHICS_SHADERS_ATTRIBUTE_IPP___

#include "Attribute.hpp"
#include "AttributeNode.hpp"

BEGIN_INANITY_SHADERS

template <typename ValueType>
Attribute<ValueType>::Attribute(int layoutSemantic)
: Value<ValueType>(NEW(AttributeNode(GetDataType<ValueType>(), layoutSemantic))) {}

END_INANITY_SHADERS

#endif
