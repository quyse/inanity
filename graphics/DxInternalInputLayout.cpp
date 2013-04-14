#include "DxInternalInputLayout.hpp"

BEGIN_INANITY_GRAPHICS

DxInternalInputLayout::DxInternalInputLayout(ComPointer<ID3D11InputLayout> inputLayout)
: inputLayout(inputLayout) {}

ID3D11InputLayout* DxInternalInputLayout::GetInputLayoutInterface() const
{
	return inputLayout;
}

END_INANITY_GRAPHICS
