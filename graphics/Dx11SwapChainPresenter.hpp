#ifndef ___INANITY_GRAPHICS_DX11_SWAP_CHAIN_PRESENTER_HPP___
#define ___INANITY_GRAPHICS_DX11_SWAP_CHAIN_PRESENTER_HPP___

#include "Dx11Presenter.hpp"
#include "DxgiMonitorMode.hpp"
#include "d3d11.hpp"
#include "../platform/platform.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_PLATFORM

class Win32Window;

END_INANITY_PLATFORM

BEGIN_INANITY_GRAPHICS

class Dx11RenderBuffer;

/// Presenter class for DirectX 11.
/** Incapsulates DXGI SwapChain. */
class Dx11SwapChainPresenter : public Dx11Presenter
{
private:
	ptr<Platform::Win32Window> window;
	ComPointer<IDXGISwapChain> swapChain;
	ptr<Dx11RenderBuffer> backBuffer;

	/// Current screen mode.
	/** Null if windowed. */
	ptr<DxgiMonitorMode> currentMode;
	/// Size of back buffer.
	int width, height;
	/// Swap interval.
	int swapInterval;

public:
	Dx11SwapChainPresenter(ptr<Dx11Device> device, ptr<Platform::Win32Window> window, ComPointer<IDXGISwapChain> swapChain);
	~Dx11SwapChainPresenter();

	//*** Dx11Presenter's methods.
	ptr<Dx11RenderBuffer> GetBackBuffer();

	//*** Presenter's methods.
	int GetWidth() const;
	int GetHeight() const;
	void SetMode(ptr<MonitorMode> mode);
	void SetSwapInterval(int swapInterval);
	void Present();
	void Resize(int width, int height);
};

END_INANITY_GRAPHICS

#endif
