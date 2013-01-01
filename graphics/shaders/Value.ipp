#ifndef ___INANITY_GRAPHICS_SHADERS_VALUE_IPP___
#define ___INANITY_GRAPHICS_SHADERS_VALUE_IPP___

#include "Value.hpp"
#include "SwizzleNode.hpp"
#include "CastNode.hpp"
#include "OperationNode.hpp"

BEGIN_INANITY_SHADERS

template <typename ValueType>
Value<ValueType>::Value(ptr<Node> node)
: Expression(node) {}

template <>
Value<float>::Value(float constValue)
: Expression(NEW(FloatConstNode(constValue))) {}

template <typename ValueType>
void Value<ValueType>::operator=(Value<ValueType> a)
{
	// реализация не требуется, оператор здесь для запрета присваивания
}

template <typename ValueType>
template <typename ResultValueType>
Value<ResultValueType> Value<ValueType>::Swizzle(const char* map) const
{
	return NEW(SwizzleNode(node, map));
}

template <typename ValueType>
template <typename CastValueType>
Value<CastValueType> Value<ValueType>::Cast() const
{
	return NEW(CastNode(GetDataType<CastValueType>(), GetNode()));
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
