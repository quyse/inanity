#include "Layout.hpp"

Layout::Element::Element(DataType dataType, int offset, int semantic)
: dataType(dataType), offset(offset), semantic(semantic) {}

Layout::Layout(const std::vector<Element>& elements, int size, int stride)
: elements(elements), size(size), stride(stride)
{
	if(this->stride < 0)
		this->stride = size;
}

const std::vector<Layout::Element>& Layout::GetElements() const
{
	return elements;
}

int Layout::GetSize() const
{
	return size;
}

int Layout::GetStride() const
{
	return stride;
}
