#ifndef ___INANITY_GRAPHICS_DX_CONTEXT_HPP___
#define ___INANITY_GRAPHICS_DX_CONTEXT_HPP___

#include "Context.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс контекста DirectX 11.
class DxContext : public Context
{
private:
	ComPointer<ID3D11DeviceContext> deviceContext;

public:
	DxContext(ID3D11DeviceContext* deviceContext);
};

END_INANITY_GRAPHICS

#endif
