#ifndef ___INANITY_GRAPHICS_DX_PRESENTER_HPP___
#define ___INANITY_GRAPHICS_DX_PRESENTER_HPP___

#include "Presenter.hpp"
#include "PresentMode.hpp"
#include "d3d.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_GRAPHICS

class DxDevice;
class Win32Output;
class DxRenderBuffer;

/// Класс Presenter для DirectX 11.
/** Инкапсулирует DXGI SwapChain. */
class DxPresenter : public Presenter
{
private:
	/// Устройство.
	ptr<DxDevice> device;
	/// Область вывода.
	ptr<Win32Output> output;
	/// Цепочка обмена буферов.
	ComPointer<IDXGISwapChain> swapChain;
	/// Вторичный буфер.
	ptr<DxRenderBuffer> backBuffer;

	/// Текущий режим экрана.
	PresentMode currentMode;

	/// Привести указанный режим экрана к самому похожему, но поддерживаемому.
	DXGI_MODE_DESC GetClosestSupportedMode(const DXGI_MODE_DESC& mode) const;

public:
	/// Создать presenter.
	DxPresenter(ptr<DxDevice> device, ptr<Win32Output> output, ComPointer<IDXGISwapChain> swapChain);
	/// Уничтожить presenter.
	~DxPresenter();

	// методы Presenter
	ptr<Device> GetDevice();
	ptr<RenderBuffer> GetBackBuffer();
	void SetMode(const PresentMode& mode);
	void Present();

	/// Изменить размер области вывода.
	/** Вызывается окном, когда его размер изменяется. */
	void Resize(size_t width, size_t height);
};

END_INANITY_GRAPHICS

#endif
