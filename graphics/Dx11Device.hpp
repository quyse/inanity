#ifndef ___INANITY_GRAPHICS_DX11_DEVICE_HPP___
#define ___INANITY_GRAPHICS_DX11_DEVICE_HPP___

#include "Device.hpp"
#include "SamplerSettings.hpp"
#include "../platform/platform.hpp"
#include "../ComPointer.hpp"
#include "d3d11.hpp"

BEGIN_INANITY_PLATFORM

class Win32Window;

END_INANITY_PLATFORM

BEGIN_INANITY_GRAPHICS

class Dx11System;
class Dx11SwapChainPresenter;
class DxgiMonitorMode;
class Dx11VertexShader;
class Dx11InternalInputLayout;
class Dx11RenderBuffer;

/// Класс графического устройства DirectX 11.
class Dx11Device : public Device
{
private:
	/// Графическая система.
	ptr<Dx11System> system;
	/// 3D-устройство.
	ComPointer<ID3D11Device> device;
	/// Контекст устройства.
	ComPointer<ID3D11DeviceContext> deviceContext;

	/// Получить ближайший поддерживаемый режим экрана.
	/** Получается для output по умолчанию (нулевого). */
	DXGI_MODE_DESC GetClosestSupportedMode(const DXGI_MODE_DESC& desc) const;

	static D3D11_TEXTURE_ADDRESS_MODE ConvertSamplerSettingsWrap(SamplerSettings::Wrap wrap);

	ptr<Dx11RenderBuffer> InternalCreateRenderBuffer(
		int width,
		int height,
		PixelFormat pixelFormat,
		const SamplerSettings& samplerSettings,
		bool gdiCompatible);

public:
	Dx11Device(ptr<Dx11System> system, ComPointer<ID3D11Device> device, ComPointer<ID3D11DeviceContext> deviceContext);

	// методы Device
	ptr<System> GetSystem() const;
	ptr<Presenter> CreateWindowPresenter(ptr<Platform::Window> window, ptr<MonitorMode> mode);
	ptr<ShaderCompiler> CreateShaderCompiler();
	ptr<Shaders::ShaderGenerator> CreateShaderGenerator();
	ptr<FrameBuffer> CreateFrameBuffer();
	ptr<RenderBuffer> CreateRenderBuffer(int width, int height, PixelFormat pixelFormat, const SamplerSettings& samplerSettings);
	ptr<DepthStencilBuffer> CreateDepthStencilBuffer(int width, int height, bool canBeResource);
	ptr<VertexShader> CreateVertexShader(ptr<File> file);
	ptr<PixelShader> CreatePixelShader(ptr<File> file);
	ptr<UniformBuffer> CreateUniformBuffer(int size);
	ptr<VertexBuffer> CreateStaticVertexBuffer(ptr<File> file, ptr<VertexLayout> layout);
	ptr<VertexBuffer> CreateDynamicVertexBuffer(int size, ptr<VertexLayout> layout);
	ptr<IndexBuffer> CreateStaticIndexBuffer(ptr<File> file, int indexSize);
	ptr<AttributeBinding> CreateAttributeBinding(ptr<AttributeLayout> layout);
	ptr<Texture> CreateStaticTexture(ptr<RawTextureData> data, const SamplerSettings& samplerSettings);
	ptr<SamplerState> CreateSamplerState(const SamplerSettings& samplerSettings);
	ptr<BlendState> CreateBlendState();

	/// Create presenter for Win32Window.
	ptr<Dx11SwapChainPresenter> CreatePresenter(ptr<Platform::Win32Window> window, ptr<DxgiMonitorMode> mode);

	/// Получить интерфейс устройства DirectX 11.
	ID3D11Device* GetDeviceInterface() const;
	/// Получить интерфейс контекста устройства DirectX 11.
	ID3D11DeviceContext* GetDeviceContextInterface() const;

	/// Create RenderBuffer with possibility of getting HDC.
	ptr<Dx11RenderBuffer> CreateRenderBufferGdiCompatible(int width, int height, const SamplerSettings& samplerSettings);
};

END_INANITY_GRAPHICS

#endif
