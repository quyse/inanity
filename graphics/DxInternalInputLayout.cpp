#include "DxInternalInputLayout.hpp"

DxInternalInputLayout::DxInternalInputLayout(ID3D11InputLayout* inputLayout)
: inputLayout(inputLayout) {}

ID3D11InputLayout* DxInternalInputLayout::GetInputLayoutInterface() const
{
	return inputLayout;
}
