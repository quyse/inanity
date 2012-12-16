#include "LValue.hpp"
#include "OperationNode.hpp"

template <typename ValueType>
LValue<ValueType>::LValue(ptr<ValueNode> node)
: Value<ValueType>(node) {}

template <typename ValueType>
Value<ValueType> LValue<ValueType>::operator=(Value<ValueType> a)
{
	return NEW(OperationNode(OperationNode::operationAssign, GetNode(), a.GetNode()));
}
