#ifndef ___INANITY_GRAPHICS_SHADERS_VALUE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_VALUE_HPP___

#include "shaders.hpp"

BEGIN_INANITY_SHADERS

class ValueNode;

/// Класс выражения определённого типа (то есть не семплер).
/** Value представляет атрибуты, константы, временные
переменные и значения. */
template <typename ValueType>
class Value
{
protected:
	/// Объект выражения.
	ptr<ValueNode> node;

private:
	/// Оператор присваивания запрещён (приватный).
	void operator=(Value a);

public:
	Value(ptr<ValueNode> node);

	ptr<ValueNode> GetNode() const;
};

END_INANITY_SHADERS

#endif
