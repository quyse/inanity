#ifndef ___INANITY_GRAPHICS_SHADERS_UNIFORM_ARRAY_IPP___
#define ___INANITY_GRAPHICS_SHADERS_UNIFORM_ARRAY_IPP___

#include "UniformArray.hpp"
#include "UniformNode.hpp"
#include "UniformGroup.hpp"
#include "../../Exception.hpp"

BEGIN_INANITY_SHADERS

template <typename ValueType>
UniformArray<ValueType>::UniformArray(ptr<UniformNode> node)
: Value<ValueType>(node.StaticCast<Node>())
{
#ifdef _DEBUG
	if(node->GetValueType() != DataTypeOf<ValueType>())
		THROW_PRIMARY_EXCEPTION("Wrong uniform node type");
#endif
}

template <typename ValueType>
Value<ValueType> UniformArray<ValueType>::operator[](Value<unsigned int> index) const
{
	return Value<ValueType>(NEW(OperationNode(OperationNode::operationIndex, this->node, index.GetNode())));
}

template <typename ValueType>
void UniformArray<ValueType>::SetValue(int index, const ValueType& value)
{
	UniformNode* uniformNode = fast_cast<UniformNode*>(&*this->node);
	*((ValueType*)((char*)uniformNode->GetGroup()->GetData() + uniformNode->GetOffset()) + index) = value;
}

END_INANITY_SHADERS

#endif
