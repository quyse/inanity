#include "AttributeLayoutSlot.hpp"
#include "AttributeLayout.hpp"
#include "VertexBuffer.hpp"

BEGIN_INANITY_GRAPHICS

AttributeLayoutSlot::AttributeLayoutSlot(ptr<AttributeLayout> layout, int index)
: layout(layout), index(index) {}

ptr<AttributeLayout> AttributeLayoutSlot::GetLayout() const
{
	return layout;
}

int AttributeLayoutSlot::GetIndex() const
{
	return index;
}

END_INANITY_GRAPHICS
