#ifndef ___INANITY_GRAPHICS_ATTRIBUTE_LAYOUT_ELEMENT_HPP___
#define ___INANITY_GRAPHICS_ATTRIBUTE_LAYOUT_ELEMENT_HPP___

#include "DataType.hpp"
#include "LayoutDataType.hpp"

BEGIN_INANITY_GRAPHICS

class AttributeLayout;

/// Element of attribute layout.
/** Just pointer + index */
class AttributeLayoutElement : public Object
{
private:
	ptr<AttributeLayout> layout;
	int index;

public:
	AttributeLayoutElement(ptr<AttributeLayout> layout, int index);

	int GetIndex() const;
	DataType GetDataType() const;
	LayoutDataType GetLayoutDataType() const;
};

END_INANITY_GRAPHICS

#endif
