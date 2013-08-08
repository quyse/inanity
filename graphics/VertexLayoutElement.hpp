#ifndef ___INANITY_GRAPHICS_VERTEX_LAYOUT_ELEMENT_HPP___
#define ___INANITY_GRAPHICS_VERTEX_LAYOUT_ELEMENT_HPP___

#include "DataType.hpp"

BEGIN_INANITY_GRAPHICS

class VertexLayout;

/// Класс элемента разметки данных вершины.
/** Внутренне содержит просто указатель на разметку вершин и индекс
в списке элементов разметки. */
class VertexLayoutElement : public Object
{
private:
	ptr<VertexLayout> layout;
	int index;

public:
	VertexLayoutElement(ptr<VertexLayout> layout, int index);

	/// Получить всю разметку.
	ptr<VertexLayout> GetLayout() const;
	/// Получить тип данных элемента.
	DataType GetDataType() const;
	/// Получить смещение до элемента от начала вершины.
	int GetOffset() const;
};

END_INANITY_GRAPHICS

#endif
