#ifndef ___INANITY_GRAPHICS_DX_CONTEXT_HPP___
#define ___INANITY_GRAPHICS_DX_CONTEXT_HPP___

#include "Context.hpp"
#include "d3d.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_GRAPHICS

class DxInternalInputLayoutCache;
class DxInternalInputLayout;
class DxVertexShader;

/// Класс контекста DirectX 11.
class DxContext : public Context
{
private:
	ComPointer<ID3D11Device> device;
	ComPointer<ID3D11DeviceContext> deviceContext;

	/// Кэш входных разметок.
	ptr<DxInternalInputLayoutCache> inputLayoutCache;
	/// Текущая входная разметка.
	ptr<DxInternalInputLayout> boundInputLayout;
	/// Получить разметку.
	ptr<DxInternalInputLayout> GetInputLayout(VertexLayout* vertexLayout, VertexShader* vertexShader);

	/// Выполнить обновление в API всех "грязных" состояний.
	void Update();

public:
	DxContext(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	/// Создать входную разметку.
	ptr<DxInternalInputLayout> CreateInternalInputLayout(VertexLayout* vertexLayout, DxVertexShader* vertexShader);

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
