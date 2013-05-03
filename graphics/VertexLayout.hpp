#ifndef ___INANITY_GRAPHICS_VERTEX_LAYOUT_HPP___
#define ___INANITY_GRAPHICS_VERTEX_LAYOUT_HPP___

#include "DataType.hpp"
#include <vector>

BEGIN_INANITY_GRAPHICS

class VertexLayoutElement;

/// Класс разметки данных в вершинном буфере.
class VertexLayout : public Object
{
public:
	/// Структура элемента вершины.
	struct Element
	{
		/// Тип данных элемента.
		DataType dataType;
		/// Смещение от начала вершины.
		int offset;

		Element(DataType dataType, int offset);
	};
	typedef std::vector<Element> Elements;

protected:
	/// Элементы разметки.
	Elements elements;
	/// Размер разметки (включающий зазор между последовательными экземплярами).
	int stride;

public:
	VertexLayout(int stride);

	/// Добавить элемент к разметке.
	ptr<VertexLayoutElement> AddElement(DataType dataType, int offset);
	/// Получить внутренние элементы.
	const Elements& GetElements() const;

	int GetStride() const;
};

END_INANITY_GRAPHICS

#endif
