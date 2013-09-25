#ifndef ___INANITY_GRAPHICS_DX11_CONTEXT_HPP___
#define ___INANITY_GRAPHICS_DX11_CONTEXT_HPP___

#include "Context.hpp"
#include "d3d11.hpp"
#include "../ComPointer.hpp"
#include <unordered_map>

BEGIN_INANITY_GRAPHICS

class Dx11VertexShader;
class Dx11AttributeBinding;

struct Dx11InputLayoutCacheKey
{
	ptr<Dx11AttributeBinding> attributeBinding;
	ptr<Dx11VertexShader> vertexShader;

	Dx11InputLayoutCacheKey(ptr<Dx11AttributeBinding> attributeBinding, ptr<Dx11VertexShader> vertexShader);
};

bool operator==(const Dx11InputLayoutCacheKey& a, const Dx11InputLayoutCacheKey& b);

END_INANITY_GRAPHICS

namespace std
{
	template <>
	struct hash<Inanity::Graphics::Dx11InputLayoutCacheKey>
	{
		size_t operator()(const Inanity::Graphics::Dx11InputLayoutCacheKey& key) const;
	};
};

BEGIN_INANITY_GRAPHICS

class Dx11Device;

/// Класс контекста DirectX 11.
class Dx11Context : public Context
{
private:
	ptr<Dx11Device> deviceObject;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	/// Кэш входных разметок по шейдерам.
	typedef std::unordered_map<Dx11InputLayoutCacheKey, ComPointer<ID3D11InputLayout> > InputLayoutCache;
	InputLayoutCache inputLayoutCache;

	/// Получить входную разметку.
	ID3D11InputLayout* GetInputLayout(ptr<Dx11AttributeBinding> attributeBinding, ptr<Dx11VertexShader> vertexShader);

	/// Закодировать состояние растеризатора, и добавить его в кэш, если ещё нет.
	int GetRasterizerStateKey(const ContextState& contextState);
	/// Максимальное количество состояний растеризатора.
	static const int rasterizerStateCacheSize = 8;
	/// Кэш состояний растеризатора.
	ComPointer<ID3D11RasterizerState> rasterizerStateCache[rasterizerStateCacheSize];

	/// Выполнить обновление в API всех "грязных" состояний.
	void Update();

	/// Указать данные буфера.
	void SetBufferData(ID3D11Resource* resource, const void* data, int size, int bufferSize);

public:
	Dx11Context(ptr<Dx11Device> deviceObject);

	// методы Context

	void ClearColor(int colorBufferIndex, const float* color);
	void ClearDepth(float depth);
	void ClearStencil(unsigned stencil);
	void ClearDepthStencil(float depth, unsigned stencil);

	void SetUniformBufferData(UniformBuffer* buffer, const void* data, int size);
	void SetVertexBufferData(VertexBuffer* buffer, const void* data, int size);

	void Draw(int count = -1);
	void DrawInstanced(int instancesCount, int count = -1);
};

END_INANITY_GRAPHICS

#endif
