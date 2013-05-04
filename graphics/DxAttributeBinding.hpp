#ifndef ___INANITY_GRAPHICS_DX_ATTRIBUTE_BINDING_HPP___
#define ___INANITY_GRAPHICS_DX_ATTRIBUTE_BINDING_HPP___

#include "AttributeBinding.hpp"
#include "d3d.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_GRAPHICS

class DxDevice;

/// Класс привязки атрибутов для DirectX.
class DxAttributeBinding : public AttributeBinding
{
private:
	ptr<DxDevice> device;
	ComPointer<ID3D11InputLayout> inputLayout;

public:
	DxAttributeBinding(ptr<DxDevice> device, ComPointer<ID3D11InputLayout> inputLayout);

	ID3D11InputLayout* GetInputLayoutInterface() const;
};

END_INANITY_GRAPHICS

#endif
