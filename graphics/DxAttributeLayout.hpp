#ifndef ___INANITY_GRAPHICS_DX_ATTRIBUTE_LAYOUT_HPP___
#define ___INANITY_GRAPHICS_DX_ATTRIBUTE_LAYOUT_HPP___

#include "AttributeLayout.hpp"
#include "d3d.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс разметки атрибутов для DirectX.
class DxAttributeLayout : public AttributeLayout
{
private:
	ComPointer<ID3D11InputLayout> inputLayout;

public:
	DxAttributeLayout(ComPointer<ID3D11InputLayout> inputLayout);

	ID3D11InputLayout* GetInputLayoutInterface() const;
};

END_INANITY_GRAPHICS

#endif
