#include "AttributeLayoutElement.hpp"
#include "AttributeLayout.hpp"

BEGIN_INANITY_GRAPHICS

AttributeLayoutElement::AttributeLayoutElement(ptr<AttributeLayout> layout, int index)
: layout(layout), index(index) {}

int AttributeLayoutElement::GetIndex() const
{
	return index;
}

DataType AttributeLayoutElement::GetDataType() const
{
	return layout->GetElements()[index].dataType;
}

END_INANITY_GRAPHICS
