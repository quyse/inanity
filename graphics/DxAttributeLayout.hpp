#ifndef ___INANITY_GRAPHICS_DX_ATTRIBUTE_LAYOUT_HPP___
#define ___INANITY_GRAPHICS_DX_ATTRIBUTE_LAYOUT_HPP___

#include "AttributeLayout.hpp"
#include "d3d.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_GRAPHICS

class DxDevice;

/// Класс разметки атрибутов для DirectX.
class DxAttributeLayout : public AttributeLayout
{
private:
	ptr<DxDevice> device;
	ComPointer<ID3D11InputLayout> inputLayout;

public:
	DxAttributeLayout(ptr<DxDevice> device);

	ID3D11InputLayout* GetInputLayoutInterface() const;

	void Finalize();
};

END_INANITY_GRAPHICS

#endif
