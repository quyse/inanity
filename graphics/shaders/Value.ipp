#ifndef ___INANITY_GRAPHICS_SHADERS_VALUE_IPP___
#define ___INANITY_GRAPHICS_SHADERS_VALUE_IPP___

#include "Value.hpp"
#include "SwizzleNode.hpp"
#include "CastNode.hpp"
#include "OperationNode.hpp"
#include "AttributeNode.hpp"
#include "../AttributeLayoutElement.hpp"

BEGIN_INANITY_SHADERS

/// Класс для получения базового типа из типа-вектора.
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

/// Класс для получения типа по длине строки.
/** Принимает базовый тип. */
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

/// Класс для получения типа вектора по длине строки.
/** Принимает тип вектора или скаляра. */
template <typename MaybeVectorType, int n>
class SwizzleHelper
{
public:
	typedef typename SwizzleTypedHelper<typename BaseTypeHelper<MaybeVectorType>::Type, n>::Type Type;
};

template <typename ValueType>
inline Value<ValueType>::Value(ptr<Node> node)
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
inline void Value<ValueType>::operator=(Value<ValueType> a)
{
	// реализация не требуется, оператор здесь для запрета присваивания
}

template <typename ValueType>
template <int n>
inline Value<typename SwizzleHelper<ValueType, n>::Type> Value<ValueType>::operator[](const char (&map)[n])
{
	return Value<typename SwizzleHelper<ValueType, n>::Type>(NEW(SwizzleNode(node, map)));
}

template <typename ValueType>
template <typename CastValueType>
inline Value<CastValueType> Value<ValueType>::Cast() const
{
	return Value<CastValueType>(NEW(CastNode(DataTypeOf<CastValueType>(), GetNode())));
}

template <typename ValueType>
inline Value<ValueType> operator-(Value<ValueType> a)
{
	return Value<ValueType>(NEW(OperationNode(OperationNode::operationNegate, a.GetNode())));
}
template <typename ValueType>
inline Value<ValueType> operator+(Value<ValueType> a, Value<ValueType> b)
{
	return Value<ValueType>(NEW(OperationNode(OperationNode::operationAdd, a.GetNode(), b.GetNode())));
}
template <typename T, int n>
inline Value<xvec<T, n> > operator+(Value<xvec<T, n> > a, Value<T> b)
{
	return Value<xvec<T, n> >(NEW(OperationNode(OperationNode::operationAdd, a.GetNode(), b.GetNode())));
}
template <typename ValueType>
inline Value<ValueType> operator-(Value<ValueType> a, Value<ValueType> b)
{
	return Value<ValueType>(NEW(OperationNode(OperationNode::operationSubtract, a.GetNode(), b.GetNode())));
}
template <typename T, int n>
inline Value<xvec<T, n> > operator-(Value<xvec<T, n> > a, Value<T> b)
{
	return Value<xvec<T, n> >(NEW(OperationNode(OperationNode::operationSubtract, a.GetNode(), b.GetNode())));
}

template <typename ValueType>
inline Value<ValueType> operator*(Value<ValueType> a, Value<ValueType> b)
{
	return Value<ValueType>(NEW(OperationNode(OperationNode::operationMultiply, a.GetNode(), b.GetNode())));
}
template <typename T, int n>
inline Value<xvec<T, n> > operator*(Value<xvec<T, n> > a, Value<T> b)
{
	return Value<xvec<T, n> >(NEW(OperationNode(OperationNode::operationMultiply, a.GetNode(), b.GetNode())));
}
template <typename T, int n>
inline Value<xvec<T, n> > operator*(Value<T> a, Value<xvec<T, n> > b)
{
	return Value<xvec<T, n> >(NEW(OperationNode(OperationNode::operationMultiply, a.GetNode(), b.GetNode())));
}

template <typename ValueType>
inline Value<ValueType> operator/(Value<ValueType> a, Value<ValueType> b)
{
	return Value<ValueType>(NEW(OperationNode(OperationNode::operationDivide, a.GetNode(), b.GetNode())));
}
template <typename T, int n>
inline Value<xvec<T, n> > operator/(Value<xvec<T, n> > a, Value<T> b)
{
	return Value<xvec<T, n> >(NEW(OperationNode(OperationNode::operationDivide, a.GetNode(), b.GetNode())));
}
template <typename T, int n>
inline Value<xvec<T, n> > operator/(Value<T> a, Value<xvec<T, n> > b)
{
	return Value<xvec<T, n> >(NEW(OperationNode(OperationNode::operationDivide, a.GetNode(), b.GetNode())));
}

template <typename ValueType>
inline Value<ValueType> operator<(Value<ValueType> a, Value<ValueType> b)
{
	return Value<ValueType>(NEW(OperationNode(OperationNode::operationLess, a.GetNode(), b.GetNode())));
}
template <typename ValueType>
inline Value<ValueType> operator<=(Value<ValueType> a, Value<ValueType> b)
{
	return Value<ValueType>(NEW(OperationNode(OperationNode::operationLessEqual, a.GetNode(), b.GetNode())));
}
template <typename ValueType>
inline Value<ValueType> operator>(Value<ValueType> a, Value<ValueType> b)
{
	return Value<ValueType>(NEW(OperationNode(OperationNode::operationLess, b.GetNode(), a.GetNode())));
}
template <typename ValueType>
inline Value<ValueType> operator>=(Value<ValueType> a, Value<ValueType> b)
{
	return Value<ValueType>(NEW(OperationNode(OperationNode::operationLessEqual, b.GetNode(), a.GetNode())));
}
template <typename ValueType>
inline Value<ValueType> operator==(Value<ValueType> a, Value<ValueType> b)
{
	return Value<ValueType>(NEW(OperationNode(OperationNode::operationEqual, a.GetNode(), b.GetNode())));
}
template <typename ValueType>
inline Value<ValueType> operator!=(Value<ValueType> a, Value<ValueType> b)
{
	return Value<ValueType>(NEW(OperationNode(OperationNode::operationNotEqual, a.GetNode(), b.GetNode())));
}

END_INANITY_SHADERS

#endif
