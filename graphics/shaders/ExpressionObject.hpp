#ifndef ___INANITY_GRAPHICS_SHADERS_EXPRESSION_OBJECT_HPP___
#define ___INANITY_GRAPHICS_SHADERS_EXPRESSION_OBJECT_HPP___

#include "shaders.hpp"

BEGIN_INANITY_SHADERS

/// Абстрактный класс объекта-выражения.
class ExpressionObject : public Object
{
public:
	/// Тип выражения.
	enum Type
	{
		/// Входная переменная.
		typeIn,
		/// Выходная переменная.
		typeOut,
		/// Константная (uniform) переменная.
		typeUniform,
		/// Семплер.
		typeSampler,
		/// Временная (локальная) переменная.
		typeTemp,
		/// Константа float.
		typeConstFloat,
		/// Константа int.
		typeConstInt,
		/// Операция унарный минус.
		typeNegate,
		/// Выбор компонент вектора.
		typeSwizzle,
		/// Присваивание.
		typeAssign,
		/// Индексирование.
		typeSubscript,
		/// Сложение.
		typeAdd,
		/// Вычитание.
		typeSubtract,
		/// Умножение.
		typeMultiply,
		/// Деление.
		typeDivide,
		/// Вызов функции с одним аргументом.
		typeCall1,
		/// Вызов функции с двумя аргументами.
		typeCall2,
		/// Вызов функции с тремя аргументами.
		typeCall3,
		/// Вызов функции с четырьмя аргументами.
		typeCall4
	};

public:
	virtual Type GetType() const = 0;
};

END_INANITY_SHADERS

#endif
