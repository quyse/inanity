#include "Dx11System.hpp"
#include "Dx11Device.hpp"
#include "Dx11Context.hpp"
#include "DxgiAdapter.hpp"
#include "DxgiMonitorMode.hpp"
#include "../platform/DllFunction.ipp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

bool Dx11System::IsSupported()
{
#if defined(___INANITY_PLATFORM_XBOX)
	return true;
#else
	try
	{
		return Platform::DllCache::Load("dxgi.dll") && Platform::DllCache::Load("d3d11.dll");
	}
	catch(Exception* exception)
	{
		MakePointer(exception);
		return false;
	}
#endif
}

#if !defined(___INANITY_PLATFORM_XBOX)
IDXGIFactory* Dx11System::GetDXGIFactory()
{
	if(!dxgiFactory)
	{
		Platform::DllFunction<decltype(&CreateDXGIFactory)> functionCreateDXGIFactory("dxgi.dll", "CreateDXGIFactory");

		if(FAILED(functionCreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&dxgiFactory)))
			THROW("Can't create DXGI Factory");
	}
	return dxgiFactory;
}
#endif

DXGI_FORMAT Dx11System::GetDXGIFormat(PixelFormat format)
{
#define T(t) PixelFormat::type##t
#define P(p) PixelFormat::pixel##p
#define F(f) PixelFormat::format##f
#define S(s) PixelFormat::size##s
#define C(c) PixelFormat::compression##c
#define X(x) DXGI_FORMAT_##x
	switch(format.type)
	{
	case T(Unknown):
		return X(UNKNOWN);
	case T(Uncompressed):
		// if sRGB, check first that format is supported (DX11 supports only one)
		if(format.srgb && (format.pixel != P(RGBA) || format.format != F(Uint) || format.size != S(32bit)))
			break;

		switch(format.pixel)
		{
		case P(R):
			switch(format.format)
			{
			case F(Untyped):
				switch(format.size)
				{
				case S(8bit): return X(R8_TYPELESS);
				case S(16bit): return X(R16_TYPELESS);
				case S(32bit): return X(R32_TYPELESS);
				};
				break;
			case F(Uint):
				switch(format.size)
				{
				case S(8bit): return X(R8_UNORM);
				case S(16bit): return X(R16_UNORM);
				}
				break;
			case F(Float):
				switch(format.size)
				{
				case S(16bit): return X(R16_FLOAT);
				case S(32bit): return X(R32_FLOAT);
				}
				break;
			}
			break;
		case P(RG):
			switch(format.format)
			{
			case F(Untyped):
				switch(format.size)
				{
				case S(16bit): return X(R8G8_TYPELESS);
				case S(32bit): return X(R16G16_TYPELESS);
				}
				break;
			case F(Uint):
				switch(format.size)
				{
				case S(16bit): return X(R8G8_UNORM);
				case S(32bit): return X(R16G16_UNORM);
				}
				break;
			case F(Float):
				switch(format.size)
				{
				case S(32bit): return X(R16G16_FLOAT);
				case S(64bit): return X(R32G32_FLOAT);
				}
				break;
			}
			break;
		case P(RGB):
			switch(format.format)
			{
			case F(Untyped):
				break;
			case F(Uint):
				break;
			case F(Float):
				switch(format.size)
				{
				case S(32bit): return X(R11G11B10_FLOAT);
				case S(96bit): return X(R32G32B32_FLOAT);
				}
				break;
			}
			break;
		case P(RGBA):
			switch(format.format)
			{
			case F(Untyped):
				switch(format.size)
				{
				case S(32bit): return X(R10G10B10A2_TYPELESS);
				case S(64bit): return X(R16G16B16A16_TYPELESS);
				case S(128bit): return X(R32G32B32A32_TYPELESS);
				}
				break;
			case F(Uint):
				switch(format.size)
				{
				case S(32bit): return format.srgb ? X(R8G8B8A8_UNORM_SRGB) : X(R8G8B8A8_UNORM);
				case S(64bit): return X(R16G16B16A16_UNORM);
				}
				break;
			case F(Float):
				switch(format.size)
				{
				case S(64bit): return X(R16G16B16A16_FLOAT);
				case S(128bit): return X(R32G32B32A32_FLOAT);
				}
				break;
			}
			break;
		}
		break;
	case T(Compressed):
		switch(format.compression)
		{
		case C(Bc1): return format.srgb ? X(BC1_UNORM_SRGB) : X(BC1_UNORM);
		case C(Bc1Alpha): break;
		case C(Bc2): return format.srgb ? X(BC2_UNORM_SRGB) : X(BC2_UNORM);
		case C(Bc3): return format.srgb ? X(BC3_UNORM_SRGB) : X(BC3_UNORM);
		case C(Bc4): return X(BC4_UNORM);
		case C(Bc4Signed): return X(BC4_SNORM);
		case C(Bc5): return X(BC5_UNORM);
		case C(Bc5Signed): return X(BC5_SNORM);
		}
		break;
	}
	THROW("Pixel format is unsupported in DX11");
#undef T
#undef P
#undef F
#undef S
#undef C
#undef X
}

DXGI_MODE_DESC Dx11System::GetModeDesc(ptr<DxgiMonitorMode> mode, int width, int height)
{
	if(mode)
		return mode->GetDesc();
	DXGI_MODE_DESC modeDesc;
	ZeroMemory(&modeDesc, sizeof(modeDesc));
	modeDesc.Width = (UINT)width;
	modeDesc.Height = (UINT)height;
	modeDesc.RefreshRate.Numerator = 0;
	modeDesc.RefreshRate.Denominator = 0;
	modeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	modeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	modeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	return modeDesc;
}

String Dx11System::GetSemanticString(int semantic)
{
	// имя семантики получается переводом кода семантики в 26-ричную систему (буквы)
	String name;
	name.assign(8, 'A');
	for(int j = 0; semantic; ++j)
	{
		name[j] = 'A' + semantic % 26;
		semantic /= 26;
	}
	return name;
}

Dx11System::Dx11System() : adaptersInitialized(false) {}

const std::vector<ptr<Adapter> >& Dx11System::GetAdapters()
{
	if(!adaptersInitialized)
		try
		{
#if !defined(___INANITY_PLATFORM_XBOX)
			IDXGIFactory* factory = GetDXGIFactory();

			if(factory)
			{
				for(UINT i = 0; ; ++i)
				{
					ComPointer<IDXGIAdapter> adapterInterface;
					HRESULT hr = factory->EnumAdapters(i, &adapterInterface);
					if(SUCCEEDED(hr))
						adapters.push_back(NEW(DxgiAdapter(adapterInterface)));
					else if(hr == DXGI_ERROR_NOT_FOUND)
						break;
					else
						THROW("Error enumerating adapters");
				}
			}
			else
#endif
				adapters.push_back(NEW(DxgiAdapter(nullptr)));

			adaptersInitialized = true;
		}
		catch(Exception* exception)
		{
			THROW_SECONDARY("Can't get adapters of DirectX 11 system", exception);
		}

	return adapters;
}

ptr<Device> Dx11System::CreateDevice(ptr<Adapter> abstractAdapter)
{
	try
	{
		// получить функцию создания в первую очередь
#if defined(___INANITY_PLATFORM_XBOX)
		auto functionD3D11CreateDevice = &D3D11CreateDevice;
#else
		Platform::DllFunction<PFN_D3D11_CREATE_DEVICE> functionD3D11CreateDevice("d3d11.dll", "D3D11CreateDevice");
#endif

		ptr<DxgiAdapter> adapter = abstractAdapter.DynamicCast<DxgiAdapter>();
		if(!adapter)
			THROW("Wrong adapter type");
		IDXGIAdapter* adapterInterface = adapter->GetInterface();

		// see https://developer.microsoft.com/en-us/games/xbox/docs/xdk/creation-flags for Xbox

		// флаги устройства
		UINT flags = 0
#ifdef _DEBUG
			| D3D11_CREATE_DEVICE_DEBUG
#endif
#if defined(___INANITY_PLATFORM_XBOX)
			| D3D11_CREATE_DEVICE_FAST_KICKOFFS
#else
			| D3D11_CREATE_DEVICE_SINGLETHREADED
#endif
			;
		// уровень устройства - DirectX 10
		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_10_0;
		// создать устройство
		ComPointer<ID3D11Device> device;
		ComPointer<ID3D11DeviceContext> deviceContext;
		D3D_FEATURE_LEVEL featureLevelSupported;
		// здесь необходимо указывать D3D_DRIVER_TYPE_UNKNOWN, если мы указываем adapter.
		// http://msdn.microsoft.com/en-us/library/ff476082 (remarks)
		if(FAILED(functionD3D11CreateDevice(adapterInterface, adapterInterface ? D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, &featureLevel, 1, D3D11_SDK_VERSION, &device, &featureLevelSupported, &deviceContext)))
			THROW("Can't create device and context");

		// вернуть объект
		return NEW(Dx11Device(this, device, deviceContext));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't create DirectX 11 device", exception);
	}
}

ptr<Context> Dx11System::CreateContext(ptr<Device> abstractDevice)
{
	BEGIN_TRY();

	ptr<Dx11Device> device = abstractDevice.DynamicCast<Dx11Device>();
	if(!device)
		THROW("Wrong device type");

	return NEW(Dx11Context(device));

	END_TRY("Can't create DirectX 11 context");
}

END_INANITY_GRAPHICS
