#include "VertexLayout.hpp"

VertexLayout::VertexLayout(int stride) : stride(stride) {}

std::vector<VertexLayout::Element>& VertexLayout::GetElements()
{
	return elements;
}
