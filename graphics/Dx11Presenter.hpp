#ifndef ___INANITY_GRAPHICS_DX11_PRESENTER_HPP___
#define ___INANITY_GRAPHICS_DX11_PRESENTER_HPP___

#include "Presenter.hpp"
#include "DxgiMonitorMode.hpp"
#include "d3d11.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_GRAPHICS

class Dx11Device;
class Win32Output;
class Dx11RenderBuffer;

/// Класс Presenter для DirectX 11.
/** Инкапсулирует DXGI SwapChain. */
class Dx11Presenter : public Presenter
{
private:
	/// Устройство.
	ptr<Dx11Device> device;
	/// Область вывода.
	ptr<Win32Output> output;
	/// Цепочка обмена буферов.
	ComPointer<IDXGISwapChain> swapChain;
	/// Вторичный буфер.
	ptr<Dx11RenderBuffer> backBuffer;

	/// Текущий режим экрана.
	/** 0, если оконный. */
	ptr<DxgiMonitorMode> currentMode;

public:
	/// Создать presenter.
	Dx11Presenter(ptr<Dx11Device> device, ptr<Win32Output> output, ComPointer<IDXGISwapChain> swapChain);
	/// Уничтожить presenter.
	~Dx11Presenter();

	// методы Presenter
	ptr<Device> GetDevice();
	ptr<RenderBuffer> GetBackBuffer();
	void SetMode(ptr<MonitorMode> mode);
	void Present();
	void Resize(int width, int height);
};

END_INANITY_GRAPHICS

#endif
