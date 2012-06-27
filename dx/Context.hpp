#ifndef ___INANITY_DX_CONTEXT_HPP___
#define ___INANITY_DX_CONTEXT_HPP___

#include "dx.hpp"
#include "d3d.hpp"
#include "../../ComPointer.hpp"
#include <vector>

BEGIN_INANITY_DX

class RenderBuffer;
class DepthStencilBuffer;
class ConstantBuffer;
class BlendState;
class DepthStencilState;
class RasterizerState;
class SamplerState;
class Texture;
class Geometry;
class ShaderLayout;
class VertexShader;
class PixelShader;

class Context : public Object
{
public:
	/// Стадии шейдерного конвейера.
	enum Stage
	{
		/// Вершинный шейдер.
		stageVertex = 1,
		/// Пиксельный шейдер.
		stagePixel = 2
	};

private:
	ComPointer<ID3D11DeviceContext> context;
	size_t lastViewportWidth, lastViewportHeight;
	std::vector<ID3D11RenderTargetView*> renderTargets;

public:
	/// Создать графический контекст.
	Context(ID3D11DeviceContext* context);

	ID3D11DeviceContext* GetDeviceContext() const;

	void SetViewport(size_t width, size_t height);

	/// Очистить рендербуфер.
	void ClearRenderBuffer(RenderBuffer* renderBuffer, const float* color);
	/// Очистить глубину в depth-stencil буфере.
	void ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, float depth);
	/// Очистить трафарет в depth-stencil буфере.
	void ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, unsigned stencil);
	/// Очистить глубину и трафарет в depth-stencil буфере.
	void ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, float depth, unsigned stencil);

	/// Указать цели для рендеринга.
	void SetRenderTargets(const std::vector<ptr<RenderBuffer> >& renderBuffers, ptr<DepthStencilBuffer> depthStencilBuffer);

	/// Скопировать текстурный буфер в другой текстурный буфер.
//	void CopyTextureBuffers(TextureBuffer* sourceBuffer, TextureBuffer* destBuffer);

	/// Указать данные буферу.
	void SetBufferData(ConstantBuffer* buffer, const void* data, size_t size);

	/// Указать blend state.
	void SetBlendState(BlendState* blendState);
	/// Указать depth-stencil state.
	void SetDepthStencilState(DepthStencilState* depthStencilState);
	/// Указать rasterizer state.
	void SetRasterizerState(RasterizerState* rasterizerState);

	/// Указать sampler state.
	void SetSamplerState(unsigned slot, unsigned stages, SamplerState* samplerState);
	/// Указать sampler states.
	void SetSamplerStates(unsigned startSlot, unsigned stages, SamplerState** samplerStates, unsigned count);

	/// Указать текстуру.
	void SetTexture(unsigned slot, unsigned stages, Texture* texture);
	/// Указать текстуры (до 4-х, прямо в параметрах)
	void SetTextures(unsigned startSlot, unsigned stages, Texture* texture1, Texture* texture2 = nullptr, Texture* texture3 = nullptr, Texture* texture4 = nullptr);
	/// Указать текстуры (неограниченно, массивом)
	void SetTextures(unsigned startSlot, unsigned stages, Texture** textures, unsigned count);

	/// Указать константный буфер.
	void SetConstantBuffer(unsigned slot, unsigned stages, ConstantBuffer* buffer);
	/// Указать константные буферы (до 4-х, прямо в параметрах)
	void SetConstantBuffers(unsigned startSlot, unsigned stages, ConstantBuffer* buffer1, ConstantBuffer* buffer2 = nullptr, ConstantBuffer* buffer3 = nullptr, ConstantBuffer* buffer4 = nullptr);
	/// Указать константные буферы (неограниченно, массивом)
	void SetConstantBuffers(unsigned startSlot, unsigned stages, ConstantBuffer** buffers, unsigned count);

	/// Применить геометрию.
	void ApplyGeometry(Geometry* geometry);
	/// Указать разметку шейдера.
	void ApplyShaderLayout(ShaderLayout* shaderLayout);
	/// Указать шейдеры.
	void ApplyShaders(VertexShader* vertexShader, PixelShader* pixelShader);
	/// Нарисовать геометрию.
	void DrawGeometry(unsigned indicesCount, bool indexed, unsigned instancesCount = 1);
};

END_INANITY_DX

#endif
