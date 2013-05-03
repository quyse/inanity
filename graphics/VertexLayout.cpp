#include "VertexLayout.hpp"
#include "VertexLayoutElement.hpp"

BEGIN_INANITY_GRAPHICS

VertexLayout::Element::Element(DataType dataType, int offset)
: dataType(dataType), offset(offset) {}

VertexLayout::VertexLayout(int stride)
: stride(stride) {}

ptr<VertexLayoutElement> VertexLayout::AddElement(DataType dataType, int offset)
{
	int index = (int)elements.size();
	elements.push_back(Element(dataType, offset));
	return NEW(VertexLayoutElement(this, index));
}

const VertexLayout::Elements& VertexLayout::GetElements() const
{
	return elements;
}

int VertexLayout::GetStride() const
{
	return stride;
}

END_INANITY_GRAPHICS
