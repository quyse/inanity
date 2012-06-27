#ifndef ___INANITY_DX_DEPTH_STENCIL_BUFFER_HPP___
#define ___INANITY_DX_DEPTH_STENCIL_BUFFER_HPP___

#include "dx.hpp"
#include "d3d.hpp"
#include "../../ComPointer.hpp"

BEGIN_INANITY_DX

class Texture;
/// Класс, олицетворяющий буфер глубины вместе с буфером трафарета.
class DepthStencilBuffer : public Object
{
private:
	ComPointer<ID3D11DepthStencilView> depthStencilView;
	ptr<Texture> texture;

public:
	DepthStencilBuffer(ID3D11DepthStencilView* depthStencilView, ptr<Texture> texture = 0);

	ID3D11DepthStencilView* GetDepthStencilView() const;
	ptr<Texture> GetAsTexture();
};

END_INANITY_DX

#endif
