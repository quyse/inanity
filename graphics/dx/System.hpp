#ifndef ___INANITY_DX_SYSTEM_HPP___
#define ___INANITY_DX_SYSTEM_HPP___

#include "../../ComPointer.hpp"
#include "dx.hpp"
#include "../SystemSettings.hpp"
#include "d3d.hpp"
#include <string>
#include <vector>

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_GRAPHICS

class Window;

END_INANITY_GRAPHICS

BEGIN_INANITY_DX

/// Класс, управляющий графикой.
/** Достаточно низкоуровневый класс. Содержит методы для создания различных ресурсов, буферов и
тому подобного. */
class Context;
class RenderBuffer;
class Texture;
class DepthStencilBuffer;
class ConstantBuffer;
class Geometry;
class GeometryFormat;
class EditableGeometry;
class OcclusionPredicate;
class VertexShader;
class PixelShader;
class System : public Object
{
public:
	enum PixelFormat
	{
		pixelFormatUnknown = DXGI_FORMAT_UNKNOWN,
		pixelFormatIntR8G8B8A8 = DXGI_FORMAT_R8G8B8A8_UNORM,
		pixelFormatFloatR11G11B10 = DXGI_FORMAT_R11G11B10_FLOAT,
		pixelFormatTypelessR32 = DXGI_FORMAT_R32_TYPELESS,
		pixelFormatFloatR32 = DXGI_FORMAT_R32_FLOAT,
		pixelFormatFloatR32Depth = DXGI_FORMAT_D32_FLOAT,
		pixelFormatIntRGBA32 = pixelFormatIntR8G8B8A8,
		pixelFormatFloatRGB32 = pixelFormatFloatR11G11B10
	};

	static const size_t maxRenderTargetCount = D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT;

private:
	static const DXGI_FORMAT screenFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	//3D-устройство
	ComPointer<ID3D11Device> device;
	/// Настройки экрана.
	Graphics::ScreenSettings screenSettings;
	//цепочка обмена буферов
	ComPointer<IDXGISwapChain> swapChain;
	//основной графический контекст
	ptr<Context> context;
	/// Вторичный буфер.
	ptr<RenderBuffer> backBuffer;

private:
	/// Перевести настроки экрана в настройки для DirectX.
	static DXGI_MODE_DESC GetScreenMode(const ScreenSettings& screenSettings);
	/// Получить настройки DirectX, которые лучше всего подходят для заданных настроек.
	static DXGI_MODE_DESC GetSupportedScreenMode(const DXGI_MODE_DESC& modeDesc);

public:
	System(ptr<Window> window, const SystemSettings& settings);

	/// Получить настройки экрана.
	const ScreenSettings& GetScreenSettings() const;
	/// Изменить настройки экрана.
	void SetScreenSettings(const ScreenSettings& screenSettings);
	/// Метод, вызываемый Graphics::Window как реакция на изменение размеров окна.
	void Resize(size_t width, size_t height);
	ptr<RenderBuffer> GetBackBuffer();
	void Flip();

	/// Получить графический контекст.
	ptr<Context> GetContext() const;

	/// Создать рендербуфер.
	ptr<RenderBuffer> CreateRenderBuffer(size_t width, size_t height, PixelFormat format);
	/// Создать буфер глубины-трафарета.
	ptr<DepthStencilBuffer> CreateDepthStencilBuffer(size_t width, size_t height, PixelFormat format, bool canBeResource = false, PixelFormat depthStencilViewFormat = pixelFormatUnknown, PixelFormat shaderResourceViewFormat = pixelFormatUnknown);
/*
	/// Создать константный буфер.
	ptr<ConstantBuffer> CreateConstantBuffer(size_t size);
	/// Создать статичную геометрию.
	ptr<Geometry> CreateStaticGeometry(ptr<File> vertices, unsigned vertexStride, ptr<File> indices, unsigned indexStride, ptr<GeometryFormat> format);

	/// Создать текстурный буфер. - непонятно зачем нужный метод.
	ptr<TextureBuffer> CreateTextureBuffer(unsigned width, unsigned height, DXGI_FORMAT format, bool canBeReadByCPU);
	std::vector<ptr<RenderBuffer>> CreateRenderBuffersMipmap(unsigned width, unsigned height, unsigned levelsCount, DXGI_FORMAT format, bool canBeResource);
	std::pair<ptr<DepthStencilBuffer>, ptr<DepthStencilBuffer>> Create2DepthStencilBuffers(unsigned width, unsigned height, DXGI_FORMAT format, bool canBeResource, DXGI_FORMAT depthStencilViewFormat1 = DXGI_FORMAT_UNKNOWN, DXGI_FORMAT depthStencilViewFormat2 = DXGI_FORMAT_UNKNOWN, DXGI_FORMAT shaderResourceViewFormat = DXGI_FORMAT_UNKNOWN);
	ptr<OcclusionPredicate> CreateOcclusionPredicate();
	/// Создать вершинный шейдер.
	ptr<VertexShader> CreateVertexShader(ptr<File> file);
	/// Создать пиксельный шейдер.
	ptr<PixelShader> CreatePixelShader(ptr<File> file);

	ID3D11Device* GetDevice() const;*/
};

END_INANITY_DX

#endif
