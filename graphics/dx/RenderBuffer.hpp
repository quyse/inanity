#ifndef ___INANITY_DX_RENDER_BUFFER_HPP___
#define ___INANITY_DX_RENDER_BUFFER_HPP___

#include "dx.hpp"
#include "d3d.hpp"

BEGIN_INANITY_DX

/// Класс рендербуфера.
class RenderBuffer : public Object
{
private:
	ComPointer<ID3D11RenderTargetView> renderTargetView;

public:
	RenderBuffer(ID3D11RenderTargetView* renderTargetView);

	ID3D11RenderTargetView* GetRenderTargetView() const;
};

END_INANITY_DX

#endif
