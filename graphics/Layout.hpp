#ifndef ___INANITY_GRAPHICS_LAYOUT_HPP___
#define ___INANITY_GRAPHICS_LAYOUT_HPP___

#include "DataType.hpp"
#include <vector>

BEGIN_INANITY_GRAPHICS

/// Класс, описывающий, где и как валяются данные в буфере.
class Layout : public Object
{
public:
	/// Структура элемента вершины.
	struct Element
	{
		/// Тип данных элемента.
		DataType dataType;
		/// Смещение от начала вершины.
		int offset;
		/// Семантика элемента.
		/** Это просто число, которое имеет смысл положения, текстурных координат,
		нормали или ещё чего-то. Соответствует Shaders::Semantics::Custom0 + номер. */
		int semantic;

		Element(DataType dataType, int offset, int semantic);
	};

protected:
	/// Элементы разметки.
	std::vector<Element> elements;
	/// Размер полезных данных разметки.
	int size;
	/// Размер разметки (включающий зазор между последовательными экземплярами).
	int stride;

public:
	Layout(const std::vector<Element>& elements, int size, int stride = -1);

	const std::vector<Element>& GetElements() const;
	int GetSize() const;
	int GetStride() const;
};

END_INANITY_GRAPHICS

#endif
