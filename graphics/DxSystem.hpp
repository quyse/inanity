#ifndef ___INANITY_GRAPHICS_DX_SYSTEM_HPP___
#define ___INANITY_GRAPHICS_DX_SYSTEM_HPP___

#include "System.hpp"
#include "PixelFormat.hpp"
#include "d3d.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_GRAPHICS

/// Графическая система, основанная на DirectX 11.
class DxSystem : public System
{
private:
	/// Фабрика объектов DXGI.
	ComPointer<IDXGIFactory> dxgiFactory;

public:
	/// Преобразовать формат пикселей из общего перечисления в DXGI.
	static DXGI_FORMAT GetDXGIFormat(PixelFormat format);

	// методы System
	ptr<Window> CreateWindow();
	ptr<Device> CreatePrimaryDevice();

	/// Получить фабрику DXGI.
	/** Создаёт её, если ещё не создана. */
	IDXGIFactory* GetDXGIFactory();
};

END_INANITY_GRAPHICS

#endif
