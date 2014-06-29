#ifndef ___INANITY_GRAPHICS_SHADERS_VALUE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_VALUE_HPP___

#include "Expression.hpp"

BEGIN_INANITY_GRAPHICS

class AttributeLayoutElement;

END_INANITY_GRAPHICS

BEGIN_INANITY_SHADERS

class ValueNode;

/// Helper for swizzle operation.
template <typename MaybeVectorType, int n>
class SwizzleHelper;

/// Explicitly typed expression.
template <typename ValueType>
class Value : public Expression
{
public:
	Value(ptr<ValueNode> node = nullptr);
	Value(ValueType constValue);
	Value(ptr<AttributeLayoutElement> element);

	ptr<ValueNode> GetNode() const;

	/// Cast value to another type.
	template <typename CastValueType>
	Value<CastValueType> Cast() const;

	/// Swizzle vector components.
	template <int n>
	Value<typename SwizzleHelper<ValueType, n>::Type> operator[](const char (&map)[n]) const;

	void operator+=(const Value<ValueType>& b);
	void operator-=(const Value<ValueType>& b);
	void operator*=(const Value<ValueType>& b);
	void operator/=(const Value<ValueType>& b);
};

END_INANITY_SHADERS

#endif
