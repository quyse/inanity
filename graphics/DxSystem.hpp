#ifndef ___INANITY_GRAPHICS_DX_SYSTEM_HPP___
#define ___INANITY_GRAPHICS_DX_SYSTEM_HPP___

#include "System.hpp"
#include "PixelFormat.hpp"
#include "PresentMode.hpp"
#include "d3d.hpp"
#include "../ComPointer.hpp"
#include "../String.hpp"

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
	/// Преобразовать режим экрана в структуру для DXGI.
	static DXGI_MODE_DESC GetModeDesc(const PresentMode& mode);
	/// Преобразовать номер сематники атрибута в строку.
	static String GetSemanticString(int semantic);

	// методы System
	ptr<Window> CreateDefaultWindow();
	ptr<Device> CreatePrimaryDevice();
	ptr<ShaderCompiler> CreateShaderCompiler();
	ptr<Shaders::ShaderGenerator> CreateShaderGenerator();

	/// Получить фабрику DXGI.
	/** Создаёт её, если ещё не создана. */
	IDXGIFactory* GetDXGIFactory();
};

END_INANITY_GRAPHICS

#endif
