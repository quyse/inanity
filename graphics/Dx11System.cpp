#include "Dx11System.hpp"
#include "Dx11Device.hpp"
#include "Dx11Context.hpp"
#include "Dx11ShaderCompiler.hpp"
#include "shaders/Hlsl11Generator.hpp"
#include "DxgiAdapter.hpp"
#include "DxgiMonitorMode.hpp"
#include "Output.hpp"
#include "../platform/DllFunction.ipp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

bool Dx11System::IsSupported()
{
	try
	{
		return DllCache::Load("dxgi.dll") && DllCache::Load("d3d11.dll");
	}
	catch(Exception* exception)
	{
		MakePointer(exception);
		return false;
	}
}

IDXGIFactory* Dx11System::GetDXGIFactory()
{
	if(!dxgiFactory)
	{
		Platform::DllFunction<decltype(&CreateDXGIFactory)> functionCreateDXGIFactory("dxgi.dll", "CreateDXGIFactory");

		IDXGIFactory* dxgiFactoryInterface;
		if(FAILED(functionCreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&dxgiFactoryInterface)))
			THROW_PRIMARY_EXCEPTION("Can't create DXGI Factory");
		dxgiFactory = dxgiFactoryInterface;
	}
	return dxgiFactory;
}

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
				case S(32bit): return X(R8G8B8A8_UNORM);
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
		case C(Dxt1): return X(BC1_UNORM);
		case C(Dxt2): return X(BC2_UNORM);
		case C(Dxt3): return X(BC3_UNORM);
		case C(Dxt4): return X(BC4_UNORM);
		case C(Dxt5): return X(BC5_UNORM);
		}
		break;
	}
	THROW_PRIMARY_EXCEPTION("Pixel format is unsupported in DX11");
#undef T
#undef P
#undef F
#undef S
#undef C
#undef X
}

DXGI_MODE_DESC Dx11System::GetModeDesc(ptr<DxgiMonitorMode> mode, ptr<Output> output)
{
	if(mode)
		return mode->GetDesc();
	DXGI_MODE_DESC modeDesc;
	ZeroMemory(&modeDesc, sizeof(modeDesc));
	modeDesc.Width = (UINT)output->GetWidth();
	modeDesc.Height = (UINT)output->GetHeight();
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
			IDXGIFactory* factory = GetDXGIFactory();

			for(UINT i = 0; ; ++i)
			{
				IDXGIAdapter* adapterInterface;
				HRESULT hr = factory->EnumAdapters(i, &adapterInterface);
				if(SUCCEEDED(hr))
					adapters.push_back(NEW(DxgiAdapter(adapterInterface)));
				else if(hr == DXGI_ERROR_NOT_FOUND)
					break;
				else
					THROW_PRIMARY_EXCEPTION("Error enumerating adapters");
			}

			adaptersInitialized = true;
		}
		catch(Exception* exception)
		{
			THROW_SECONDARY_EXCEPTION("Can't get adapters of DirectX 11 system", exception);
		}

	return adapters;
}

ptr<Device> Dx11System::CreateDevice(ptr<Adapter> abstractAdapter)
{
	try
	{
		// получить функцию создания в первую очередь
		Platform::DllFunction<PFN_D3D11_CREATE_DEVICE> functionD3D11CreateDevice("d3d11.dll", "D3D11CreateDevice");

		ptr<DxgiAdapter> adapter = abstractAdapter.DynamicCast<DxgiAdapter>();
		if(!adapter)
			THROW_PRIMARY_EXCEPTION("Wrong adapter type");
		IDXGIAdapter* adapterInterface = adapter->GetInterface();

		// флаги устройства
		UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED
#ifdef _DEBUG
			| D3D11_CREATE_DEVICE_DEBUG
#endif
			;
		// уровень устройства - DirectX 10
		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_10_0;
		// создать устройство
		ID3D11Device* deviceInterface;
		ID3D11DeviceContext* deviceContextInterface;
		D3D_FEATURE_LEVEL featureLevelSupported;
		// здесь необходимо указывать D3D_DRIVER_TYPE_UNKNOWN, если мы указываем adapter.
		// http://msdn.microsoft.com/en-us/library/ff476082 (remarks)
		if(FAILED(functionD3D11CreateDevice(adapterInterface, D3D_DRIVER_TYPE_UNKNOWN, NULL, flags, &featureLevel, 1, D3D11_SDK_VERSION, &deviceInterface, &featureLevelSupported, &deviceContextInterface)))
			THROW_PRIMARY_EXCEPTION("Can't create device and context");

		ComPointer<ID3D11Device> device = deviceInterface;
		ComPointer<ID3D11DeviceContext> deviceContext = deviceContextInterface;

		ptr<Dx11Context> context = NEW(Dx11Context(device, deviceContext));

		// вернуть объект
		return NEW(Dx11Device(this, device, context));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create primary device", exception);
	}
}

ptr<ShaderCompiler> Dx11System::CreateShaderCompiler()
{
	return NEW(Dx11ShaderCompiler());
}

ptr<Shaders::ShaderGenerator> Dx11System::CreateShaderGenerator()
{
	return NEW(Shaders::Hlsl11Generator());
}

END_INANITY_GRAPHICS
