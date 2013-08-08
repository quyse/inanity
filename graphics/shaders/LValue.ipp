#ifndef ___INANITY_GRAPHICS_SHADERS_LVALUE_IPP___
#define ___INANITY_GRAPHICS_SHADERS_LVALUE_IPP___

#include "LValue.hpp"
#include "OperationNode.hpp"

BEGIN_INANITY_SHADERS

template <typename ValueType>
LValue<ValueType>::LValue(ptr<Node> node)
: Value<ValueType>(node) {}

template <typename ValueType>
Value<ValueType> LValue<ValueType>::operator=(LValue<ValueType> a)
{
	return Value<ValueType>(NEW(OperationNode(OperationNode::operationAssign, this->GetNode(), a.GetNode())));
}

template <typename ValueType>
Value<ValueType> LValue<ValueType>::operator=(Value<ValueType> a)
{
	return Value<ValueType>(NEW(OperationNode(OperationNode::operationAssign, this->GetNode(), a.GetNode())));
}

END_INANITY_SHADERS

#endif
