#ifndef ___INANITY_GRAPHICS_SHADERS_LVALUE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_LVALUE_HPP___

#include "Value.hpp"

BEGIN_INANITY_SHADERS

/// Класс L-value значения.
/** То есть такого, которое можно присваивать. */
template <typename ValueType>
class LValue : public Value<ValueType>
{
public:
	LValue(ptr<ValueNode> node);

	/// Оператор присваивания.
	Value<ValueType> operator=(Value<ValueType> a);
};

END_INANITY_SHADERS

#endif
