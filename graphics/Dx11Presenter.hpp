#ifndef ___INANITY_GRAPHICS_DX11_PRESENTER_HPP___
#define ___INANITY_GRAPHICS_DX11_PRESENTER_HPP___

#include "Presenter.hpp"
#include "DxgiMonitorMode.hpp"
#include "d3d11.hpp"
#include "../ComPointer.hpp"

BEGIN_INANITY_GRAPHICS

class Dx11Device;
class Win32Output;
class Dx11FrameBuffer;
class Dx11RenderBuffer;

/// Presenter class for DirectX 11.
/** Incapsulates DXGI SwapChain. */
class Dx11Presenter : public Presenter
{
private:
	ptr<Dx11Device> device;
	ptr<Win32Output> output;
	ComPointer<IDXGISwapChain> swapChain;
	ptr<Dx11FrameBuffer> frameBuffer;
	ptr<Dx11RenderBuffer> backBuffer;

	/// Current screen mode.
	/** Null if windowed. */
	ptr<DxgiMonitorMode> currentMode;

public:
	Dx11Presenter(ptr<Dx11Device> device, ptr<Win32Output> output, ComPointer<IDXGISwapChain> swapChain);
	~Dx11Presenter();

	ptr<Dx11RenderBuffer> Dx11Presenter::GetBackBuffer();

	//*** Presenter's methods.
	ptr<Device> GetDevice() const;
	ptr<FrameBuffer> GetFrameBuffer() const;
	void SetMode(ptr<MonitorMode> mode);
	void Present();
	void Resize(int width, int height);
};

END_INANITY_GRAPHICS

#endif
