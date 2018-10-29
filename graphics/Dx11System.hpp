#ifndef ___INANITY_GRAPHICS_DX11_SYSTEM_HPP___
#define ___INANITY_GRAPHICS_DX11_SYSTEM_HPP___

#include "System.hpp"
#include "PixelFormat.hpp"
#include "d3d11.hpp"
#include "../ComPointer.hpp"
#include "../String.hpp"

BEGIN_INANITY_GRAPHICS

class DxgiMonitorMode;

/// Графическая система, основанная на DirectX 11.
class Dx11System : public System
{
private:
	/// Фабрика объектов DXGI.
#if !defined(___INANITY_PLATFORM_XBOX)
	ComPointer<IDXGIFactory> dxgiFactory;
#endif
	/// Адаптеры DXGI.
	std::vector<ptr<Adapter> > adapters;
	bool adaptersInitialized;

public:
	/// Преобразовать формат пикселей из общего перечисления в DXGI.
	static DXGI_FORMAT GetDXGIFormat(PixelFormat format);
	/// Создать структуру режима экрана DXGI.
	static DXGI_MODE_DESC GetModeDesc(ptr<DxgiMonitorMode> mode, int width, int height);
	/// Преобразовать номер сематники атрибута в строку.
	static String GetSemanticString(int semantic);

	Dx11System();

	// методы System
	const std::vector<ptr<Adapter> >& GetAdapters();
	ptr<Device> CreateDevice(ptr<Adapter> adapter);
	ptr<Context> CreateContext(ptr<Device> device);

	/// Checks if DirectX 11 is supported.
	static bool IsSupported();

	/// Получить фабрику DXGI.
	/** Создаёт её, если ещё не создана. */
#if !defined(___INANITY_PLATFORM_XBOX)
	IDXGIFactory* GetDXGIFactory();
#endif
};

END_INANITY_GRAPHICS

#endif
