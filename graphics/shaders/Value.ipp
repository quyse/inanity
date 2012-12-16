#ifndef ___INANITY_GRAPHICS_SHADERS_VALUE_IPP___
#define ___INANITY_GRAPHICS_SHADERS_VALUE_IPP___

#include "Value.hpp"
#include "OperationNode.hpp"
#include "../../Exception.hpp"

BEGIN_INANITY_SHADERS

template <typename ValueType>
Value<ValueType>::Value(ptr<ValueNode> node)
: node(node)
{
#ifdef _DEBUG
	if(node->GetValueType() != GetDataType<ValueType>())
		THROW_PRIMARY_EXCEPTION("Wrong value expression value type");
#endif
}

template <typename ValueType>
void Value<ValueType>::operator=(Value<ValueType> a)
{
	// реализация не требуется, оператор здесь для запрета присваивания
}

template <typename ValueType>
ptr<ValueNode> Value<ValueType>::GetNode() const
{
	return node;
}

template <typename ValueType>
Value<ValueType> operator-(Value<ValueType> a)
{
	return NEW(OperationNode(OperationNode::operationNegate, a.GetNode()));
}
template <typename ValueType>
Value<ValueType> operator+(Value<ValueType> a, Value<ValueType> b)
{
	return NEW(OperationNode(OperationNode::operationAdd, a.GetNode(), b.GetNode()));
}
template <typename ValueType>
Value<ValueType> operator-(Value<ValueType> a, Value<ValueType> b)
{
	return NEW(OperationNode(OperationNode::operationSubtract, a.GetNode(), b.GetNode()));
}
template <typename ValueType>
Value<ValueType> operator*(Value<ValueType> a, Value<ValueType> b)
{
	return NEW(OperationNode(OperationNode::operationMultiply, a.GetNode(), b.GetNode()));
}
template <typename ValueType>
Value<ValueType> operator/(Value<ValueType> a, Value<ValueType> b)
{
	return NEW(OperationNode(OperationNode::operationDivide, a.GetNode(), b.GetNode()));
}

END_INANITY_SHADERS

#endif
