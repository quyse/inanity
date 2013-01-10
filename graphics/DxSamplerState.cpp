#include "DxSamplerState.hpp"
#include "DxDevice.hpp"
#include "../Exception.hpp"

DxSamplerState::DxSamplerState(ptr<DxDevice> device) : device(device) {}

ID3D11SamplerState* DxSamplerState::GetSamplerStateInterface()
{
	Update();
	return samplerState;
}

D3D11_TEXTURE_ADDRESS_MODE DxSamplerState::ConvertWrap(Wrap wrap)
{
	switch(wrap)
	{
	case wrapRepeat:
		return D3D11_TEXTURE_ADDRESS_WRAP;
	case wrapRepeatMirror:
		return D3D11_TEXTURE_ADDRESS_MIRROR;
	case wrapClamp:
		return D3D11_TEXTURE_ADDRESS_CLAMP;
	case wrapBorder:
		return D3D11_TEXTURE_ADDRESS_BORDER;
	}
	THROW_PRIMARY_EXCEPTION("Invalid wrap mode");
}

void DxSamplerState::Update()
{
	if(!dirty)
		return;

	// удалить предыдущий объект
	samplerState = 0;

	try
	{
		D3D11_SAMPLER_DESC desc = CD3D11_SAMPLER_DESC(CD3D11_DEFAULT());

		// в DirectX общий параметр для фильтрации, так что разбираемся

		static bool haveFilterConversionMap = false;
		// параметры: min, mag, mip (как в DX, и не как в Sampler)
		static D3D11_FILTER filterConversionMap[filterCount][filterCount][filterCount];
		if(!haveFilterConversionMap)
		{
#ifdef _DEBUG
			// если добавили ещё фильтры, то нужно дописать таблицу ниже
			if(filterCount != 2)
				THROW_PRIMARY_EXCEPTION("Please recode conversion map");
#endif

			filterConversionMap [filterPoint]  [filterPoint]  [filterPoint]  = D3D11_FILTER_MIN_MAG_MIP_POINT;
			filterConversionMap [filterPoint]  [filterPoint]  [filterLinear] = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
			filterConversionMap [filterPoint]  [filterLinear] [filterPoint]  = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
			filterConversionMap [filterPoint]  [filterLinear] [filterLinear] = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
			filterConversionMap [filterLinear] [filterPoint]  [filterPoint]  = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
			filterConversionMap [filterLinear] [filterPoint]  [filterLinear] = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			filterConversionMap [filterLinear] [filterLinear] [filterPoint]  = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
			filterConversionMap [filterLinear] [filterLinear] [filterLinear] = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			haveFilterConversionMap = true;
		}

		// фильтрация
		desc.Filter = filterConversionMap[minFilter][magFilter][mipFilter];
		// режимы свёртки
		desc.AddressU = ConvertWrap(wrapU);
		desc.AddressV = ConvertWrap(wrapV);
		desc.AddressW = ConvertWrap(wrapW);
		// минимальный и максимальный LOD
		desc.MinLOD = minLOD;
		desc.MaxLOD = maxLOD;
		// цвет границы
		for(int i = 0; i < 4; ++i)
			desc.BorderColor[i] = borderColor[i];

		// создать sampler state
		ID3D11SamplerState* samplerStateInterface;
		if(FAILED(device->GetDeviceInterface()->CreateSamplerState(&desc, &samplerStateInterface)))
			THROW_PRIMARY_EXCEPTION("Can't create sampler state");

		samplerState = samplerStateInterface;

		dirty = false;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't update DirectX sampler", exception);
	}
}
