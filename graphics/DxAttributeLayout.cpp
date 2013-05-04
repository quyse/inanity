#include "DxAttributeLayout.hpp"

BEGIN_INANITY_GRAPHICS

DxAttributeLayout::DxAttributeLayout(ComPointer<ID3D11InputLayout> inputLayout)
: inputLayout(inputLayout) {}

ID3D11InputLayout* DxAttributeLayout::GetInputLayoutInterface() const
{
	return inputLayout;
}

END_INANITY_GRAPHICS
