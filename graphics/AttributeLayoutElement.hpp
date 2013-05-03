#ifndef ___INANITY_GRAPHICS_ATTRIBUTE_LAYOUT_ELEMENT_HPP___
#define ___INANITY_GRAPHICS_ATTRIBUTE_LAYOUT_ELEMENT_HPP___

#include "DataType.hpp"

BEGIN_INANITY_GRAPHICS

class AttributeLayout;

/// Класс элемента разметки атрибутов.
/** Внутренне содержит просто ссылку на разметку атрибутов,
и индекс элемента. */
class AttributeLayoutElement : public Object
{
private:
	ptr<AttributeLayout> layout;
	int index;

public:
	AttributeLayoutElement(ptr<AttributeLayout> layout, int index);

	int GetIndex() const;
	DataType GetDataType() const;
};

END_INANITY_GRAPHICS

#endif
