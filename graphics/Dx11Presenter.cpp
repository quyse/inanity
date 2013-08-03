#include "Dx11Presenter.hpp"
#include "Dx11System.hpp"
#include "Dx11Device.hpp"
#include "Dx11RenderBuffer.hpp"
#include "Dx11Texture.hpp"
#include "Win32Output.hpp"
#include "DxgiMonitorMode.hpp"
#include "../platform/Win32Window.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

Dx11Presenter::Dx11Presenter(ptr<Dx11Device> device, ptr<Win32Output> output, ComPointer<IDXGISwapChain> swapChain)
: device(device), output(output), swapChain(swapChain), currentMode(0)
{
	output->SetPresenter(this);
}

Dx11Presenter::~Dx11Presenter()
{
	output->SetPresenter(0);

	// перед уничтожением swap chain, необходимо выключить полноэкранность
	if(currentMode)
		swapChain->SetFullscreenState(FALSE, NULL);
}

ptr<Device> Dx11Presenter::GetDevice()
{
	return device;
}

ptr<RenderBuffer> Dx11Presenter::GetBackBuffer()
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

void Dx11Presenter::SetMode(ptr<MonitorMode> abstractMode)
{
	try
	{
		ptr<DxgiMonitorMode> mode = abstractMode.DynamicCast<DxgiMonitorMode>();
		if(!mode && abstractMode)
			THROW("Only DXGI monitor mode allowed");

		// В MSDN рекомендуется сначала изменять размер, а потом переключать полноэкранность.

		// изменить режим
		if(FAILED(swapChain->ResizeTarget(&Dx11System::GetModeDesc(mode, output))))
			THROW("Can't resize target");
		//если полноэкранность изменилась, изменить её
		if(!currentMode != !mode)
		{
			if(FAILED(swapChain->SetFullscreenState(!!mode, NULL)))
				THROW("Can't set fullscreen state");

			// и при этом нужно изменить размеры буфера
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

void Dx11Presenter::Present()
{
	if(FAILED(swapChain->Present(0, 0)))
		THROW("Can't present");
}

void Dx11Presenter::Resize(int width, int height)
{
	// сбросить ссылку на вторичный буфер
	backBuffer = 0;
	// изменить размеры буфера
	if(FAILED(swapChain->ResizeBuffers(2, width, height, Dx11System::GetModeDesc(currentMode, output).Format, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH)))
		THROW("Can't resize buffers");
}

END_INANITY_GRAPHICS
