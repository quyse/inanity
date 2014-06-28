#ifndef ___INANITY_GRAPHICS_SHADERS_UNIFORM_IPP___
#define ___INANITY_GRAPHICS_SHADERS_UNIFORM_IPP___

#include "Uniform.hpp"
#include "UniformNode.hpp"
#include "UniformGroup.hpp"
#include "ReadUniformNode.hpp"

BEGIN_INANITY_SHADERS

template <typename ValueType>
Uniform<ValueType>::Uniform(ptr<UniformNode> uniformNode)
: Value<ValueType>(uniformNode ? NEW(ReadUniformNode(uniformNode)) : nullptr), uniformNode(uniformNode)
{}

template <typename ValueType>
void Uniform<ValueType>::Set(const ValueType& value)
{
	*(ValueType*)((char*)uniformNode->GetGroup()->GetData() + uniformNode->GetOffset()) = value;
}

END_INANITY_SHADERS

#endif
