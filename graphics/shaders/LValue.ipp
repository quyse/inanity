#ifndef ___INANITY_GRAPHICS_SHADERS_LVALUE_IPP___
#define ___INANITY_GRAPHICS_SHADERS_LVALUE_IPP___

#include "LValue.hpp"
#include "OperationNode.hpp"

BEGIN_INANITY_SHADERS

template <typename ValueType>
LValue<ValueType>::LValue(ptr<Node> node)
: Value<ValueType>(node) {}

template <typename ValueType>
Value<ValueType> LValue<ValueType>::operator=(const Value<ValueType>& a)
{
	return Value<ValueType>(NEW(OperationNode(OperationNode::operationAssign, this->GetNode(), a.GetNode())));
}

template <typename ValueType>
template <int n>
inline LValue<typename SwizzleHelper<ValueType, n>::Type> LValue<ValueType>::operator[](const char (&map)[n])
{
	return LValue<typename SwizzleHelper<ValueType, n>::Type>(NEW(SwizzleNode(node, map)));
}

END_INANITY_SHADERS

#endif
