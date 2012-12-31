#ifndef ___INANITY_GRAPHICS_SHADERS_VALUE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_VALUE_HPP___

#include "Expression.hpp"

BEGIN_INANITY_SHADERS

class Node;

/// Класс выражения определённого типа (то есть не семплер).
/** Value представляет атрибуты, константы, временные
переменные и значения. */
template <typename ValueType>
class Value : public Expression
{
protected:
	/// Объект выражения.
	ptr<Node> node;

private:
	/// Оператор присваивания запрещён (приватный).
	void operator=(Value a);

public:
	Value(ptr<Node> node);
	Value(ValueType constValue);

	/// Получить перестановку компонент.
	/** Сейчас по тупому, но по другому не знаю пока.
	ResultValueType должно соответствовать строке в map. */
	template <typename ResultValueType>
	Value<ResultValueType> Swizzle(const char* map) const;
};

END_INANITY_SHADERS

#endif
