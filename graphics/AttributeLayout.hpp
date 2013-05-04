#ifndef ___INANITY_GRAPHICS_ATTRIBUTE_LAYOUT_HPP___
#define ___INANITY_GRAPHICS_ATTRIBUTE_LAYOUT_HPP___

#include "DataType.hpp"
#include <vector>

BEGIN_INANITY_GRAPHICS

class VertexLayout;
class VertexLayoutElement;
class AttributeLayoutSlot;
class AttributeLayoutElement;

/// Класс разметки атрибутов для привязки к шейдерному конвейеру.
class AttributeLayout : public Object
{
public:
	/// Структура, описывающая слот для вершинного буфера.
	struct Slot
	{
		/// Разметка вершин, которая привязана к слоту.
		ptr<VertexLayout> vertexLayout;
		/// Шаг инстансов геометрии.
		/** Для индексированного элемента - 0. Для per-instance-элемента - 1 и больше. */
		int divisor;

		Slot(ptr<VertexLayout> vertexLayout, int divisor);
	};
	typedef std::vector<Slot> Slots;

	/// Элемент разметки атрибутов.
	struct Element
	{
		/// Слот вершинного буфера.
		int slot;
		/// Смещение от начала вершины.
		int offset;
		/// Тип данных элемента.
		DataType dataType;

		Element(int slot, int offset, DataType dataType);
	};
	typedef std::vector<Element> Elements;

private:
	Slots slots;
	Elements elements;

public:
	ptr<AttributeLayoutSlot> AddSlot(ptr<VertexLayout> vertexLayout, int divisor = 0);
	ptr<AttributeLayoutElement> AddElement(ptr<AttributeLayoutSlot> slot, ptr<VertexLayoutElement> element);

	const Slots& GetSlots() const;
	const Elements& GetElements() const;
};

END_INANITY_GRAPHICS

#endif
