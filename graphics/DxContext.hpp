#ifndef ___INANITY_GRAPHICS_DX_CONTEXT_HPP___
#define ___INANITY_GRAPHICS_DX_CONTEXT_HPP___

#include "Context.hpp"
#include "d3d.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс контекста DirectX 11.
class DxContext : public Context
{
private:
	ComPointer<ID3D11DeviceContext> deviceContext;

	/// Выполнить обновление в API всех "грязных" состояний.
	void Update();

public:
	DxContext(ID3D11DeviceContext* deviceContext);

	// методы Context

	void ClearRenderBuffer(RenderBuffer* renderBuffer, const float* color);
	void ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, float depth);
	void ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, unsigned stencil);
	void ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, float depth, unsigned stencil);

	void SetUniformBufferData(UniformBuffer* buffer, const void* data, size_t size);

	void Draw();
	void DrawInstanced(int instancesCount);
};

END_INANITY_GRAPHICS

#endif
