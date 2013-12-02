#include "VertexLayoutElement.hpp"
#include "VertexLayout.hpp"

BEGIN_INANITY_GRAPHICS

VertexLayoutElement::VertexLayoutElement(ptr<VertexLayout> layout, int index)
: layout(layout), index(index) {}

ptr<VertexLayout> VertexLayoutElement::GetLayout() const
{
	return layout;
}

DataType VertexLayoutElement::GetDataType() const
{
	return layout->GetElements()[index].dataType;
}

LayoutDataType VertexLayoutElement::GetLayoutDataType() const
{
	return layout->GetElements()[index].layoutDataType;
}

int VertexLayoutElement::GetOffset() const
{
	return layout->GetElements()[index].offset;
}

END_INANITY_GRAPHICS
