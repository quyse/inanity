#ifndef ___INANITY_DX_SYSTEM_HPP___
#define ___INANITY_DX_SYSTEM_HPP___

#include "../ComPointer.hpp"
#include "dx.hpp"
#include "SystemSettings.hpp"
#include "d3d.h"
#include <string>
#include <vector>

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_DX

/// Класс, управляющий графикой.
/** Достаточно низкоуровневый класс. Содержит методы для создания различных ресурсов, буферов и
тому подобного. */
class Context;
class TextureBuffer;
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
class Graphics : public Object
{
private:
	//флаг инициализированности
	bool initialized;
	//3D-устройство
	ComPointer<ID3D11Device> device;
	/// Настройки экрана.
	ScreenSettings screenSettings;
	//цепочка обмена буферов
	ComPointer<IDXGISwapChain> swapChain;
	//основной графический контекст
	ptr<GraphicsContext> context;
	/// Вторичный буфер в виде текстуры.
	ptr<TextureBuffer> backBufferTextureBuffer;
	/// Вторичный буфер в виде рендербуфера.
	ptr<RenderBuffer> backBufferRenderBuffer;

public:
	Graphics();
	~Graphics();

	bool IsInitialized() const;
	void Initialize(const GraphicsSettings& settings);
	void Uninitialize();

	/// Получить настройки экрана, которые лучше всего подходят для заданных настроек.
	static DXGI_MODE_DESC GetSupportedScreenMode(const DXGI_MODE_DESC& modeDesc);

	/// Получить настройки экрана.
	const ScreenSettings& GetScreenSettings() const;
	/// Изменить настройки экрана.
	void SetScreenSettings(const ScreenSettings& screenSettings);
	/// Метод, вызываемый GameWindow как реакция на изменение размеров окна.
	void Resize(unsigned width, unsigned height);
	ptr<TextureBuffer> GetBackBufferTextureBuffer();
	ptr<RenderBuffer> GetBackBufferRenderBuffer();
	void Flip();

	/// Получить графический контекст.
	ptr<GraphicsContext> GetGraphicsContext() const;

	/// Создать константный буфер.
	ptr<ConstantBuffer> CreateConstantBuffer(size_t size);
	/// Создать статичную геометрию.
	ptr<Geometry> CreateStaticGeometry(ptr<File> vertices, unsigned vertexStride, ptr<File> indices, unsigned indexStride, ptr<GeometryFormat> format);

	/// Создать текстурный буфер. - непонятно зачем нужный метод.
	ptr<TextureBuffer> CreateTextureBuffer(unsigned width, unsigned height, DXGI_FORMAT format, bool canBeReadByCPU);
	/// Создать рендербуфер.
	ptr<RenderBuffer> CreateRenderBuffer(unsigned width, unsigned height, DXGI_FORMAT format, bool canBeResource);
	std::vector<ptr<RenderBuffer>> CreateRenderBuffersMipmap(unsigned width, unsigned height, unsigned levelsCount, DXGI_FORMAT format, bool canBeResource);
	ptr<DepthStencilBuffer> CreateDepthStencilBuffer(unsigned width, unsigned height, DXGI_FORMAT format, bool canBeResource, DXGI_FORMAT depthStencilViewFormat = DXGI_FORMAT_UNKNOWN, DXGI_FORMAT shaderResourceViewFormat = DXGI_FORMAT_UNKNOWN);
	std::pair<ptr<DepthStencilBuffer>, ptr<DepthStencilBuffer>> Create2DepthStencilBuffers(unsigned width, unsigned height, DXGI_FORMAT format, bool canBeResource, DXGI_FORMAT depthStencilViewFormat1 = DXGI_FORMAT_UNKNOWN, DXGI_FORMAT depthStencilViewFormat2 = DXGI_FORMAT_UNKNOWN, DXGI_FORMAT shaderResourceViewFormat = DXGI_FORMAT_UNKNOWN);
	ptr<OcclusionPredicate> CreateOcclusionPredicate();
	/// Создать вершинный шейдер.
	ptr<VertexShader> CreateVertexShader(ptr<File> file);
	/// Создать пиксельный шейдер.
	ptr<PixelShader> CreatePixelShader(ptr<File> file);

	ID3D11Device* GetDevice() const;
};

END_INANITY_DX

#endif
