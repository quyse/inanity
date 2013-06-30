#ifndef ___INANITY_GRAPHICS_DX11_DEVICE_HPP___
#define ___INANITY_GRAPHICS_DX11_DEVICE_HPP___

#include "Device.hpp"
#include "../ComPointer.hpp"
#include "d3d11.hpp"

BEGIN_INANITY_GRAPHICS

class Dx11System;
class Dx11Context;
class Dx11VertexShader;
class Dx11InternalInputLayout;

/// Класс графического устройства DirectX 11.
class Dx11Device : public Device
{
private:
	/// Графическая система.
	ptr<Dx11System> system;
	/// 3D-адаптер.
	ComPointer<IDXGIAdapter> adapter;
	/// 3D-устройство.
	ComPointer<ID3D11Device> device;
	/// Основной графический контекст.
	ptr<Dx11Context> context;

	/// Получить ближайший поддерживаемый режим экрана.
	/** Получается для output по умолчанию (нулевого). */
	DXGI_MODE_DESC GetClosestSupportedMode(const DXGI_MODE_DESC& desc) const;

public:
	Dx11Device(ptr<Dx11System> system, ComPointer<IDXGIAdapter> adapter, ComPointer<ID3D11Device> device, ptr<Dx11Context> context);

	// методы Device
	ptr<System> GetSystem() const;
	ptr<Presenter> CreatePresenter(ptr<Output> output, const PresentMode& mode);
	ptr<Context> GetContext();
	ptr<RenderBuffer> CreateRenderBuffer(int width, int height, PixelFormat pixelFormat);
	ptr<DepthStencilBuffer> CreateDepthStencilBuffer(int width, int height, bool canBeResource);
	ptr<VertexShader> CreateVertexShader(ptr<File> file);
	ptr<PixelShader> CreatePixelShader(ptr<File> file);
	ptr<UniformBuffer> CreateUniformBuffer(int size);
	ptr<VertexBuffer> CreateStaticVertexBuffer(ptr<File> file, ptr<VertexLayout> layout);
	ptr<VertexBuffer> CreateDynamicVertexBuffer(int size, ptr<VertexLayout> layout);
	ptr<IndexBuffer> CreateStaticIndexBuffer(ptr<File> file, int indexSize);
	ptr<AttributeBinding> CreateAttributeBinding(ptr<AttributeLayout> layout);
	ptr<Texture> CreateStaticTexture(ptr<File> file);
	ptr<Texture> CreateStatic2DTexture(ptr<Image2DData> imageData);
	ptr<SamplerState> CreateSamplerState();
	ptr<BlendState> CreateBlendState();

	/// Получить интерфейс устройства DirectX 11.
	ID3D11Device* GetDeviceInterface() const;
};

END_INANITY_GRAPHICS

#endif
