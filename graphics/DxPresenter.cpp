#include "DxPresenter.hpp"
#include "DxSystem.hpp"
#include "DxDevice.hpp"
#include "DxRenderBuffer.hpp"
#include "DxTexture.hpp"
#include "Win32Output.hpp"
#include "../Exception.hpp"

DxPresenter::DxPresenter(ptr<DxDevice> device, ptr<Win32Output> output, const PresentMode& mode)
: device(device), output(output)
{
	try
	{
		// сформировать структуру настроек swap chain
		DXGI_SWAP_CHAIN_DESC desc;

		// подзадача - сформировать структуру режима экрана
		DXGI_MODE_DESC& modeDesc = desc.BufferDesc;
		modeDesc = GetModeDesc(mode);
		// если режим - полноэкранный, необходимо его скорректировать,
		// то есть привести к поддерживаемому
		if(mode.fullscreen)
			modeDesc = GetClosestSupportedMode(modeDesc);

		// мультисемплинга пока нет
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 2;
		desc.OutputWindow = output->GetWindowHandle();
		// согласно рекомендации SDK, даже в случае полного экрана, лучше
		// создавать в оконном режиме, а потом переключать
		desc.Windowed = TRUE;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		// создать swap chain
		IDXGISwapChain* swapChainInterface;
		if(FAILED(device->GetSystem().StaticCast<DxSystem>()->GetDXGIFactory()->CreateSwapChain(device->GetDeviceInterface(), &desc, &swapChainInterface)))
			THROW_PRIMARY_EXCEPTION("Can't create swap chain");

		swapChain = swapChainInterface;

		/* Есть некая странная процедура отсюда:
		http://msdn.microsoft.com/en-us/library/ee417025(v=VS.85).aspx#Full_Screen_Issues,
		но она не работает в полноэкранном режиме. Поэтому сделано так.
		*/
		// текущий режим поставим неправильный, чтобы потом он переключился
		currentMode.width = 1;
		currentMode.height = 1;
		currentMode.fullscreen = true;
		currentMode.pixelFormat = mode.pixelFormat;
		// переключить режим на тот, который нужен
		SetMode(mode);

		// всё
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create DXGI Presenter", exception);
	}
}

DXGI_MODE_DESC DxPresenter::GetModeDesc(const PresentMode& mode)
{
	DXGI_MODE_DESC modeDesc;
	modeDesc.Width = (UINT)mode.width;
	modeDesc.Height = (UINT)mode.height;
	modeDesc.Format = DxSystem::GetDXGIFormat(mode.pixelFormat);
	modeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	modeDesc.RefreshRate.Numerator = 0;
	modeDesc.RefreshRate.Denominator = 0;
	modeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	return modeDesc;
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
		// В MSDN рекомендуется сначала изменять размер, а потом переключать полноэкранность.

		// если режим изменился, изменить его
		if(currentMode.width != mode.width || currentMode.height != mode.height)
			if(FAILED(swapChain->ResizeTarget(&GetModeDesc(mode))))
				THROW_PRIMARY_EXCEPTION("Can't resize target");
		//если полноэкранность изменилась, изменить её
		if(currentMode.fullscreen != mode.fullscreen)
			if(FAILED(swapChain->SetFullscreenState(mode.fullscreen, NULL)))
				THROW_PRIMARY_EXCEPTION("Can't set fullscreen state");

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

void DxPresenter::Resize(size_t width, size_t height)
{
	// сбросить ссылку на вторичный буфер
	backBuffer = 0;
	// изменить размеры буфера
	if(FAILED(swapChain->ResizeBuffers(1, width, height, DxSystem::GetDXGIFormat(currentMode.pixelFormat), DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH)))
		THROW_PRIMARY_EXCEPTION("Can't resize buffers");
}
