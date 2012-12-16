#ifndef ___INANITY_GRAPHICS_SHADERS_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_NODE_HPP___

#include "../DataType.hpp"
#include "shaders.hpp"

BEGIN_INANITY_SHADERS

/// Абстрактный класс выражения.
class Node : public Object
{
public:
	/// Тип выражения.
	/** Типы соответствуют классам, к которым можно привести класс. */
	enum Type
	{
		/// Константа.
		typeConst,
		/// Атрибут.
		typeAttribute,
		/// Uniform-переменная.
		typeUniform,
		/// Временная переменная.
		typeTemp,
		/// Выход вершинного шейдера (и вход пиксельного).
		typeTransformed,
		/// Выход пиксельного шейдера.
		typeRasterized,
		/// Операция без аргументов.
		typeOperation0,
		/// Операция с одним аргументом.
		typeOperation1,
		/// Операция с двумя аргументами.
		typeOperation2,
		/// Операция с тремя аргументами.
		typeOperation3,
		/// Операция с четырьмя аргументами.
		typeOperation4,
		/// Семплирование.
		typeSample
	};

public:
	/// Получить тип класса.
	virtual Type GetType() const = 0;
};

END_INANITY_SHADERS

#endif
