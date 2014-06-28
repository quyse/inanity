#ifndef ___INANITY_GRAPHICS_SHADERS_UNIFORM_ARRAY_IPP___
#define ___INANITY_GRAPHICS_SHADERS_UNIFORM_ARRAY_IPP___

#include "UniformArray.hpp"
#include "UniformNode.hpp"
#include "UniformGroup.hpp"
#include "IndexUniformArrayNode.hpp"

BEGIN_INANITY_SHADERS

template <typename ValueType>
UniformArray<ValueType>::UniformArray(ptr<UniformNode> uniformNode)
: uniformNode(uniformNode)
{}

template <typename ValueType>
Value<ValueType> UniformArray<ValueType>::operator[](Value<uint> index) const
{
	return Value<ValueType>(NEW(IndexUniformArrayNode(uniformNode, index.GetNode())));
}

template <typename ValueType>
void UniformArray<ValueType>::Set(int index, const ValueType& value)
{
	*((ValueType*)((char*)uniformNode->GetGroup()->GetData() + uniformNode->GetOffset()) + index) = value;
}

END_INANITY_SHADERS

#endif
