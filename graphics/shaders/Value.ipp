#ifndef ___INANITY_GRAPHICS_SHADERS_VALUE_IPP___
#define ___INANITY_GRAPHICS_SHADERS_VALUE_IPP___

#include "Value.hpp"
#include "SwizzleNode.hpp"
#include "CastNode.hpp"
#include "OperationNode.hpp"

BEGIN_INANITY_SHADERS

/// Класс для получения базового типа из типа-вектора.
template <typename MaybeVectorType>
class BaseTypeHelper;
template <int n>
class BaseTypeHelper<vector<n> >
{
public:
	typedef scalar Type;
};
template <>
class BaseTypeHelper<scalar>
{
public:
	typedef scalar Type;
};
template <int n>
class BaseTypeHelper<uintvector<n> >
{
public:
	typedef uint Type;
};
template <>
class BaseTypeHelper<uint>
{
public:
	typedef uint Type;
};

/// Класс для получения типа по длине строки.
/** Принимает базовый тип. */
template <typename BaseType, int n>
class SwizzleTypedHelper;
template <int n>
class SwizzleTypedHelper<float, n>
{
public:
	typedef vector<n - 1> Type;
};
template <>
class SwizzleTypedHelper<float, 2>
{
public:
	typedef scalar Type;
};
template <int n>
class SwizzleTypedHelper<uint, n>
{
public:
	typedef uintvector<n - 1> Type;
};
template <>
class SwizzleTypedHelper<uint, 2>
{
public:
	typedef uint Type;
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
	return Value<CastValueType>(NEW(CastNode(GetDataType<CastValueType>(), GetNode())));
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
template <int n>
inline Value<vector<n> > operator+(Value<vector<n> > a, Value<float> b)
{
	return Value<vector<n> >(NEW(OperationNode(OperationNode::operationAdd, a.GetNode(), b.GetNode())));
}
template <typename ValueType>
inline Value<ValueType> operator-(Value<ValueType> a, Value<ValueType> b)
{
	return Value<ValueType>(NEW(OperationNode(OperationNode::operationSubtract, a.GetNode(), b.GetNode())));
}
template <int n>
inline Value<vector<n> > operator-(Value<vector<n> > a, Value<float> b)
{
	return Value<vector<n> >(NEW(OperationNode(OperationNode::operationSubtract, a.GetNode(), b.GetNode())));
}

template <typename ValueType>
inline Value<ValueType> operator*(Value<ValueType> a, Value<ValueType> b)
{
	return Value<ValueType>(NEW(OperationNode(OperationNode::operationMultiply, a.GetNode(), b.GetNode())));
}
template <int n>
inline Value<vector<n> > operator*(Value<vector<n> > a, Value<float> b)
{
	return Value<vector<n> >(NEW(OperationNode(OperationNode::operationMultiply, a.GetNode(), b.GetNode())));
}
template <int n>
inline Value<vector<n> > operator*(Value<float> a, Value<vector<n> > b)
{
	return Value<vector<n> >(NEW(OperationNode(OperationNode::operationMultiply, a.GetNode(), b.GetNode())));
}

template <typename ValueType>
inline Value<ValueType> operator/(Value<ValueType> a, Value<ValueType> b)
{
	return Value<ValueType>(NEW(OperationNode(OperationNode::operationDivide, a.GetNode(), b.GetNode())));
}
template <int n>
inline Value<vector<n> > operator/(Value<vector<n> > a, Value<float> b)
{
	return Value<vector<n> >(NEW(OperationNode(OperationNode::operationDivide, a.GetNode(), b.GetNode())));
}
template <int n>
inline Value<vector<n> > operator/(Value<float> a, Value<vector<n> > b)
{
	return Value<vector<n> >(NEW(OperationNode(OperationNode::operationDivide, a.GetNode(), b.GetNode())));
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
