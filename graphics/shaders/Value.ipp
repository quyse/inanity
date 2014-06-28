#ifndef ___INANITY_GRAPHICS_SHADERS_VALUE_IPP___
#define ___INANITY_GRAPHICS_SHADERS_VALUE_IPP___

#include "Value.hpp"
#include "ValueNode.hpp"
#include "FloatConstNode.hpp"
#include "IntConstNode.hpp"
#include "SwizzleNode.hpp"
#include "CastNode.hpp"
#include "OperationNode.hpp"
#include "AttributeNode.hpp"
#include "SequenceNode.hpp"
#include "../AttributeLayoutElement.hpp"

BEGIN_INANITY_SHADERS

/// Helper to get base type from vector type.
template <typename MaybeVectorType>
class BaseTypeHelper
{
public:
	typedef MaybeVectorType Type;
};
template <typename T, int n>
class BaseTypeHelper<xvec<T, n> >
{
public:
	typedef T Type;
};

/// Helper to get type by string length.
/** T is base type. */
template <typename T, int n>
class SwizzleTypedHelper
{
public:
	typedef xvec<T, n - 1> Type;
};
template <typename T>
class SwizzleTypedHelper<T, 2>
{
public:
	typedef T Type;
};

template <typename MaybeVectorType, int n>
class SwizzleHelper
{
public:
	typedef typename SwizzleTypedHelper<typename BaseTypeHelper<MaybeVectorType>::Type, n>::Type Type;
};

template <typename ValueType>
inline Value<ValueType>::Value(ptr<ValueNode> node)
: Expression(node) {}

template <>
inline Value<float>::Value(float constValue)
: Expression(NEW(FloatConstNode(constValue))) {}

template <>
inline Value<int>::Value(int constValue)
: Expression(NEW(IntConstNode(constValue))) {}

template <>
inline Value<uint>::Value(uint constValue)
: Expression(NEW(IntConstNode(constValue))) {}

template <typename ValueType>
inline Value<ValueType>::Value(ptr<AttributeLayoutElement> element)
: Expression(NEW(AttributeNode(element))) {}

template <typename ValueType>
inline ptr<ValueNode> Value<ValueType>::GetNode() const
{
	return node.FastCast<ValueNode>();
}

template <typename ValueType>
template <int n>
inline Value<typename SwizzleHelper<ValueType, n>::Type> Value<ValueType>::operator[](const char (&map)[n])
{
	return Value<typename SwizzleHelper<ValueType, n>::Type>(NEW(SwizzleNode(GetNode(), map)));
}

template <typename ValueType>
template <typename CastValueType>
inline Value<CastValueType> Value<ValueType>::Cast() const
{
	return Value<CastValueType>(NEW(CastNode(DataTypeOf<CastValueType>(), GetNode())));
}

template <typename ValueType>
inline Value<ValueType> operator-(const Value<ValueType>& a)
{
	return Value<ValueType>(NEW(OperationNode(OperationNode::operationNegate, DataTypeOf<ValueType>(), a.GetNode())));
}
template <typename ValueType>
inline Value<ValueType> operator+(const Value<ValueType>& a, const Value<ValueType>& b)
{
	return Value<ValueType>(NEW(OperationNode(OperationNode::operationAdd, DataTypeOf<ValueType>(), a.GetNode(), b.GetNode())));
}
template <typename T, int n>
inline Value<xvec<T, n> > operator+(const Value<xvec<T, n> >& a, const Value<T>& b)
{
	return Value<xvec<T, n> >(NEW(OperationNode(OperationNode::operationAdd, DataTypeOf<xvec<T, n> >(), a.GetNode(), b.GetNode())));
}
template <typename ValueType>
inline Value<ValueType> operator-(const Value<ValueType>& a, const Value<ValueType>& b)
{
	return Value<ValueType>(NEW(OperationNode(OperationNode::operationSubtract, DataTypeOf<ValueType>(), a.GetNode(), b.GetNode())));
}
template <typename T, int n>
inline Value<xvec<T, n> > operator-(const Value<xvec<T, n> >& a, const Value<T>& b)
{
	return Value<xvec<T, n> >(NEW(OperationNode(OperationNode::operationSubtract, DataTypeOf<xvec<T, n> >(), a.GetNode(), b.GetNode())));
}

template <typename ValueType>
inline Value<ValueType> operator*(const Value<ValueType>& a, const Value<ValueType>& b)
{
	return Value<ValueType>(NEW(OperationNode(OperationNode::operationMultiply, DataTypeOf<ValueType>(), a.GetNode(), b.GetNode())));
}
template <typename T, int n>
inline Value<xvec<T, n> > operator*(const Value<xvec<T, n> >& a, const Value<T>& b)
{
	return Value<xvec<T, n> >(NEW(OperationNode(OperationNode::operationMultiply, DataTypeOf<xvec<T, n> >(), a.GetNode(), b.GetNode())));
}
template <typename T, int n>
inline Value<xvec<T, n> > operator*(const Value<T>& a, const Value<xvec<T, n> >& b)
{
	return Value<xvec<T, n> >(NEW(OperationNode(OperationNode::operationMultiply, DataTypeOf<xvec<T, n> >(), a.GetNode(), b.GetNode())));
}

template <typename ValueType>
inline Value<ValueType> operator/(const Value<ValueType>& a, const Value<ValueType>& b)
{
	return Value<ValueType>(NEW(OperationNode(OperationNode::operationDivide, DataTypeOf<ValueType>(), a.GetNode(), b.GetNode())));
}
template <typename T, int n>
inline Value<xvec<T, n> > operator/(const Value<xvec<T, n> >& a, const Value<T>& b)
{
	return Value<xvec<T, n> >(NEW(OperationNode(OperationNode::operationDivide, DataTypeOf<xvec<T, n> >(), a.GetNode(), b.GetNode())));
}
template <typename T, int n>
inline Value<xvec<T, n> > operator/(const Value<T>& a, const Value<xvec<T, n> >& b)
{
	return Value<xvec<T, n> >(NEW(OperationNode(OperationNode::operationDivide, DataTypeOf<xvec<T, n> >(), a.GetNode(), b.GetNode())));
}

template <typename ValueType>
inline Value<bool> operator<(const Value<ValueType>& a, const Value<ValueType>& b)
{
	return Value<bool>(ptr<ValueNode>(NEW(OperationNode(OperationNode::operationLess, DataTypes::_bool, a.GetNode(), b.GetNode()))));
}
template <typename ValueType>
inline Value<bool> operator<=(const Value<ValueType>& a, const Value<ValueType>& b)
{
	return Value<bool>(ptr<ValueNode>(NEW(OperationNode(OperationNode::operationLessEqual, DataTypes::_bool, a.GetNode(), b.GetNode()))));
}
template <typename ValueType>
inline Value<bool> operator>(const Value<ValueType>& a, const Value<ValueType>& b)
{
	return Value<bool>(ptr<ValueNode>(NEW(OperationNode(OperationNode::operationLess, DataTypes::_bool, b.GetNode(), a.GetNode()))));
}
template <typename ValueType>
inline Value<bool> operator>=(const Value<ValueType>& a, const Value<ValueType>& b)
{
	return Value<bool>(ptr<ValueNode>(NEW(OperationNode(OperationNode::operationLessEqual, DataTypes::_bool, b.GetNode(), a.GetNode()))));
}
template <typename ValueType>
inline Value<bool> operator==(const Value<ValueType>& a, const Value<ValueType>& b)
{
	return Value<bool>(ptr<ValueNode>(NEW(OperationNode(OperationNode::operationEqual, DataTypes::_bool, a.GetNode(), b.GetNode()))));
}
template <typename ValueType>
inline Value<bool> operator!=(const Value<ValueType>& a, const Value<ValueType>& b)
{
	return Value<bool>(ptr<ValueNode>(NEW(OperationNode(OperationNode::operationNotEqual, DataTypes::_bool, a.GetNode(), b.GetNode()))));
}

template <typename ValueType>
inline Value<ValueType> operator,(const Expression& a, const Value<ValueType>& b)
{
	return NEW(SequenceNode(a.GetNode(), b.GetNode()));
}

template <typename ValueType>
inline void Value<ValueType>::operator+=(const Value<ValueType>& b)
{
	*this = *this + b;
}
template <typename ValueType>
inline void Value<ValueType>::operator-=(const Value<ValueType>& b)
{
	*this = *this - b;
}
template <typename ValueType>
inline void Value<ValueType>::operator*=(const Value<ValueType>& b)
{
	*this = *this * b;
}
template <typename ValueType>
inline void Value<ValueType>::operator/=(const Value<ValueType>& b)
{
	*this = *this / b;
}

END_INANITY_SHADERS

#endif
