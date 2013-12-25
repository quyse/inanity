#include "Dx11SwapChainPresenter.hpp"
#include "Dx11System.hpp"
#include "Dx11Device.hpp"
#include "Dx11FrameBuffer.hpp"
#include "Dx11RenderBuffer.hpp"
#include "Dx11Texture.hpp"
#include "DxgiMonitorMode.hpp"
#include "../platform/Win32Window.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

Dx11SwapChainPresenter::Dx11SwapChainPresenter(ptr<Dx11Device> device, ptr<Platform::Win32Window> window, ComPointer<IDXGISwapChain> swapChain)
: Dx11Presenter(device), window(window), swapChain(swapChain), currentMode(0)
{
	window->SetPresenter(this);
	width = window->GetClientWidth();
	height = window->GetClientHeight();
}

Dx11SwapChainPresenter::~Dx11SwapChainPresenter()
{
	window->SetPresenter(nullptr);

	// switch off fullscreeness before destroying
	if(currentMode)
		swapChain->SetFullscreenState(FALSE, NULL);
}

ptr<Dx11RenderBuffer> Dx11SwapChainPresenter::GetBackBuffer()
{
	if(!backBuffer)
	{
		ComPointer<ID3D11Texture2D> backBufferTexture;
		if(FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTexture)))
			THROW("Can't get back buffer texture");

		ID3D11Device* deviceInterface = device->GetDeviceInterface();

		ComPointer<ID3D11RenderTargetView> renderTarget;
		if(FAILED(deviceInterface->CreateRenderTargetView(backBufferTexture, 0, &renderTarget)))
			THROW("Can't create back buffer render target view");
		backBuffer = NEW(Dx11RenderBuffer(renderTarget));
	}
	return backBuffer;
}

int Dx11SwapChainPresenter::GetWidth() const
{
	return width;
}

int Dx11SwapChainPresenter::GetHeight() const
{
	return height;
}

void Dx11SwapChainPresenter::SetMode(ptr<MonitorMode> abstractMode)
{
	try
	{
		ptr<DxgiMonitorMode> mode = abstractMode.DynamicCast<DxgiMonitorMode>();
		if(!mode && abstractMode)
			THROW("Only DXGI monitor mode allowed");

		// В MSDN рекомендуется сначала изменять размер, а потом переключать полноэкранность.

		// изменить режим
		if(FAILED(swapChain->ResizeTarget(&Dx11System::GetModeDesc(mode, width, height))))
			THROW("Can't resize target");
		// если полноэкранность изменилась, изменить её
		if(!currentMode != !mode)
		{
			if(FAILED(swapChain->SetFullscreenState(!!mode, NULL)))
				THROW("Can't set fullscreen state");

			// if it becomes fullscreen, change buffers size
			if(mode)
				Resize(mode->GetWidth(), mode->GetHeight());
		}

		//запомнить новый режим
		currentMode = mode;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't set mode", exception);
	}
}

void Dx11SwapChainPresenter::Present()
{
	if(FAILED(swapChain->Present(0, 0)))
		THROW("Can't present");
}

void Dx11SwapChainPresenter::Resize(int width, int height)
{
	// drop reference to back buffer
	backBuffer = 0;
	// remember new size
	this->width = width;
	this->height = height;
	// resize buffers
	if(FAILED(swapChain->ResizeBuffers(2, width, height, Dx11System::GetModeDesc(currentMode, width, height).Format, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH)))
		THROW("Can't resize buffers");
}

END_INANITY_GRAPHICS
