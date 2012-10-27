#ifndef ___INANITY_GRAPHICS_LAYOUT_HPP___
#define ___INANITY_GRAPHICS_LAYOUT_HPP___

#include "DataType.hpp"

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
		нормали или ещё чего-то. */
		int semantic;

		Element(DataType dataType, int offset, int semantic);
	};

protected:
	/// Элементы разметки.
	std::vector<Element> elements;
	/// Размер буфера.
	int size;

public:
	Layout(const std::vector<Element>& elements, int size);

	const std::vector<Element>& GetElements() const;
	int GetSize() const;
};

END_INANITY_GRAPHICS

#endif
