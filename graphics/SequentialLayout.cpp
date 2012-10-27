#include "SequentialLayout.hpp"

SequentialLayout::SequentialLayout(const std::vector<Element>& elements, int size, int stride)
: Layout(elements, size), stride(stride) {}

int SequentialLayout::GetStride() const
{
	return stride;
}
