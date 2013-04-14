#include "DxPresenter.hpp"
#include "DxSystem.hpp"
#include "DxDevice.hpp"
#include "DxRenderBuffer.hpp"
#include "DxTexture.hpp"
#include "Win32Output.hpp"
#include "../Win32Window.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

DxPresenter::DxPresenter(ptr<DxDevice> device, ptr<Win32Output> output, ComPointer<IDXGISwapChain> swapChain)
: device(device), output(output), swapChain(swapChain)
{
	try
	{
		// текущий режим поставим неправильный, чтобы потом он переключился
		currentMode.width = 1;
		currentMode.height = 1;
		currentMode.fullscreen = false;

		// указать себя окну
		output->GetWindow()->SetGraphicsPresenter(this);
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create DXGI Presenter", exception);
	}
}

DxPresenter::~DxPresenter()
{
	// отменить указание себя окну
	output->GetWindow()->SetGraphicsPresenter(0);

	// перед уничтожением swap chain, необходимо выключить полноэкранность
	if(currentMode.fullscreen)
		swapChain->SetFullscreenState(FALSE, NULL);
}

DXGI_MODE_DESC DxPresenter::GetClosestSupportedMode(const DXGI_MODE_DESC& mode) const
{
	try
	{
		IDXGIOutput* dxgiOutputInterface;
		if(FAILED(swapChain->GetContainingOutput(&dxgiOutputInterface)))
			THROW_PRIMARY_EXCEPTION("Can't get containing output for swap chain");

		ComPointer<IDXGIOutput> dxgiOutput = dxgiOutputInterface;

		DXGI_MODE_DESC closestMode;
		if(FAILED(dxgiOutput->FindClosestMatchingMode(&mode, &closestMode, device->GetDeviceInterface())))
			THROW_PRIMARY_EXCEPTION("Can't get closest matching mode");

		return closestMode;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't get closest supported mode", exception);
	}
}

ptr<Device> DxPresenter::GetDevice()
{
	return device;
}

ptr<RenderBuffer> DxPresenter::GetBackBuffer()
{
	if(!backBuffer)
	{
		ID3D11Texture2D* backBufferTextureInterface;
		if(FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTextureInterface)))
			THROW_PRIMARY_EXCEPTION("Can't get back buffer texture");

		ID3D11Device* deviceInterface = device->GetDeviceInterface();

		ID3D11RenderTargetView* renderTargetInterface;
		HRESULT hr = deviceInterface->CreateRenderTargetView(backBufferTextureInterface, 0, &renderTargetInterface);
		backBufferTextureInterface->Release();
		if(FAILED(hr))
			THROW_PRIMARY_EXCEPTION("Can't create back buffer render target view");
		backBuffer = NEW(DxRenderBuffer(renderTargetInterface));
	}
	return backBuffer;
}

void DxPresenter::SetMode(const PresentMode& mode)
{
	try
	{
		// получить режим DXGI
		DXGI_MODE_DESC desc = DxSystem::GetModeDesc(mode);
		// если режим полноэкранный, его нужно привести к поддерживаемому
		if(mode.fullscreen)
			desc = GetClosestSupportedMode(desc);

		// В MSDN рекомендуется сначала изменять размер, а потом переключать полноэкранность.

		// если режим изменился, изменить его
		if(currentMode.width != mode.width || currentMode.height != mode.height)
			if(FAILED(swapChain->ResizeTarget(&desc)))
				THROW_PRIMARY_EXCEPTION("Can't resize target");
		//если полноэкранность изменилась, изменить её
		if(currentMode.fullscreen != mode.fullscreen)
		{
			if(FAILED(swapChain->SetFullscreenState(mode.fullscreen, NULL)))
				THROW_PRIMARY_EXCEPTION("Can't set fullscreen state");

			// и при этом нужно изменить размеры буфера
			Resize(mode.width, mode.height);
		}

		//запомнить новый режим
		currentMode = mode;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't set mode", exception);
	}
}

void DxPresenter::Present()
{
	if(FAILED(swapChain->Present(0, 0)))
		THROW_PRIMARY_EXCEPTION("Can't present");
}

void DxPresenter::Resize(int width, int height)
{
	// сбросить ссылку на вторичный буфер
	backBuffer = 0;
	// изменить размеры буфера
	if(FAILED(swapChain->ResizeBuffers(2, width, height, DxSystem::GetDXGIFormat(currentMode.pixelFormat), DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH)))
		THROW_PRIMARY_EXCEPTION("Can't resize buffers");
}

END_INANITY_GRAPHICS
