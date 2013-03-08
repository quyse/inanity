#ifndef ___INANITY_GRAPHICS_SHADERS_VALUE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_VALUE_HPP___

#include "Expression.hpp"

BEGIN_INANITY_SHADERS

class Node;

/// Вспомогательный класс для операции перестановки компонент.
template <typename MaybeVectorType, int n>
class SwizzleHelper;

/// Класс выражения определённого типа (то есть не семплер).
/** Value представляет атрибуты, константы, временные
переменные и значения. */
template <typename ValueType>
class Value : public Expression
{
private:
	/// Оператор присваивания запрещён (приватный).
	void operator=(Value a);

public:
	Value(ptr<Node> node);
	Value(ValueType constValue);

	/// Преобразовать тип.
	template <typename CastValueType>
	Value<CastValueType> Cast() const;

	/// Перестановка компонент (для вектора).
	template <int n>
	Value<typename SwizzleHelper<ValueType, n>::Type> operator[](const char (&map)[n]);
};

END_INANITY_SHADERS

#endif
