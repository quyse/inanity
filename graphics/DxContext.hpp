#ifndef ___INANITY_GRAPHICS_DX_CONTEXT_HPP___
#define ___INANITY_GRAPHICS_DX_CONTEXT_HPP___

#include "Context.hpp"
#include "d3d.hpp"
#include "../ComPointer.hpp"
#include <unordered_map>

BEGIN_INANITY_GRAPHICS

class DxVertexShader;
class DxAttributeBinding;

struct DxInputLayoutCacheKey
{
	ptr<DxAttributeBinding> attributeBinding;
	ptr<DxVertexShader> vertexShader;

	DxInputLayoutCacheKey(ptr<DxAttributeBinding> attributeBinding, ptr<DxVertexShader> vertexShader);
};

bool operator==(const DxInputLayoutCacheKey& a, const DxInputLayoutCacheKey& b);

END_INANITY_GRAPHICS

namespace std
{
	template <>
	struct hash<Inanity::Graphics::DxInputLayoutCacheKey>
	{
		size_t operator()(const Inanity::Graphics::DxInputLayoutCacheKey& key) const;
	};
};

BEGIN_INANITY_GRAPHICS

/// Класс контекста DirectX 11.
class DxContext : public Context
{
private:
	ComPointer<ID3D11Device> device;
	ComPointer<ID3D11DeviceContext> deviceContext;

	/// Кэш входных разметок по шейдерам.
	typedef std::unordered_map<DxInputLayoutCacheKey, ComPointer<ID3D11InputLayout> > InputLayoutCache;
	InputLayoutCache inputLayoutCache;

	/// Получить входную разметку.
	ID3D11InputLayout* GetInputLayout(ptr<DxAttributeBinding> attributeBinding, ptr<DxVertexShader> vertexShader);

	/// Закодировать состояние растеризатора, и добавить его в кэш, если ещё нет.
	int GetRasterizerStateKey(const ContextState& contextState);
	/// Максимальное количество состояний растеризатора.
	static const int rasterizerStateCacheSize = 8;
	/// Кэш состояний растеризатора.
	ComPointer<ID3D11RasterizerState> rasterizerStateCache[rasterizerStateCacheSize];

	/// Выполнить обновление в API всех "грязных" состояний.
	void Update();

public:
	DxContext(ComPointer<ID3D11Device> device, ComPointer<ID3D11DeviceContext> deviceContext);

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
