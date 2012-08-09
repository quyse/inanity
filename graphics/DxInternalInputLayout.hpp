#ifndef ___INANITY_GRAPHICS_DX_INTERNAL_INPUT_LAYOUT_HPP___
#define ___INANITY_GRAPHICS_DX_INTERNAL_INPUT_LAYOUT_HPP___

#include "graphics.hpp"
#include "d3d.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс для DX, хранящий привязку конкретного VertexLayout к конкретному шейдеру.
class DxInternalInputLayout : public Object
{
private:
	ComPointer<ID3D11InputLayout> inputLayout;

public:
	DxInternalInputLayout(ID3D11InputLayout* inputLayout);

	ID3D11InputLayout* GetInputLayoutInterface() const;
};

END_INANITY_GRAPHICS

#endif
