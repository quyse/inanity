#ifndef ___INANITY_DX_RENDER_BUFFER_HPP___
#define ___INANITY_DX_RENDER_BUFFER_HPP___

#include "dx.hpp"
#include "d3d.hpp"
#include "../../ComPointer.hpp"

BEGIN_INANITY_DX

class Texture;

/// Класс рендербуфера.
class RenderBuffer : public Object
{
private:
	size_t width, height;
	ComPointer<ID3D11RenderTargetView> renderTargetView;
	ptr<Texture> texture;

public:
	RenderBuffer(ID3D11RenderTargetView* renderTargetView, ptr<Texture> texture = 0);

	size_t GetWidth() const;
	size_t GetHeight() const;
	ID3D11RenderTargetView* GetRenderTargetView() const;
	ptr<Texture> GetAsTexture() const;
};

END_INANITY_DX

#endif
