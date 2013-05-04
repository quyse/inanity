#include "DxAttributeBinding.hpp"
#include "DxDevice.hpp"
#include "DxSystem.hpp"
#include "VertexLayout.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

DxAttributeBinding::DxAttributeBinding(ptr<DxDevice> device, ComPointer<ID3D11InputLayout> inputLayout)
: device(device), inputLayout(inputLayout) {}

ID3D11InputLayout* DxAttributeBinding::GetInputLayoutInterface() const
{
	return inputLayout;
}

END_INANITY_GRAPHICS
