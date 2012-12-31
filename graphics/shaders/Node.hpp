#ifndef ___INANITY_GRAPHICS_SHADERS_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_NODE_HPP___

#include "shaders.hpp"
#include "../DataType.hpp"

BEGIN_INANITY_SHADERS

/// Абстрактный класс выражения.
class Node : public Object
{
public:
	/// Тип выражения.
	/** Типы соответствуют классам, к которым можно привести класс. */
	enum Type
	{
		/// Константа float.
		typeFloatConst,
		/// Атрибут.
		typeAttribute,
		/// Uniform-переменная.
		typeUniform,
		/// Семплер.
		typeSampler,
		/// Временная переменная.
		typeTemp,
		/// Выходная переменная вершинного шейдера.
		typeTransformed,
		/// Выходная переменная пиксельного шейдера.
		typeRasterized,
		/// Последовательность выражений.
		typeSequence,
		/// Операция перестановки компонент.
		typeSwizzle,
		/// Операция.
		typeOperation,
		/// Семплирование.
		typeSample
	};

public:
	/// Получить тип класса.
	virtual Type GetType() const = 0;
};

END_INANITY_SHADERS

#endif
