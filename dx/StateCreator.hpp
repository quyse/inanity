#ifndef ___INANITY_DX_STATE_CREATOR_HPP___
#define ___INANITY_DX_STATE_CREATOR_HPP___

#include "dx.hpp"
#include "d3d.hpp"

BEGIN_INANITY_DX

/// Шаблонный класс для создания интерфейсов графических стейтов.
/** Основан на том, что объявлены специализации для конкретных типов интерфейсов.
Специализации содержат метод Create, который и выполняет создание интерфейса.
Create не выбрасывает исключений, а возвращает нулевой указатель в случае неудачи. */
template <typename Interface>
class StateCreator
{
};

template <>
class StateCreator<ID3D11BlendState>
{
public:
	static ID3D11BlendState* Create(ID3D11Device* device, const D3D11_BLEND_DESC& desc)
	{
		ID3D11BlendState* state;
		return SUCCEEDED(device->CreateBlendState(&desc, &state)) ? state : 0;
	}
};

template <>
class StateCreator<ID3D11DepthStencilState>
{
public:
	static ID3D11DepthStencilState* Create(ID3D11Device* device, const D3D11_DEPTH_STENCIL_DESC& desc)
	{
		ID3D11DepthStencilState* state;
		return SUCCEEDED(device->CreateDepthStencilState(&desc, &state)) ? state : 0;
	}
};

template <>
class StateCreator<ID3D11RasterizerState>
{
public:
	static ID3D11RasterizerState* Create(ID3D11Device* device, const D3D11_RASTERIZER_DESC& desc)
	{
		ID3D11RasterizerState* state;
		return SUCCEEDED(device->CreateRasterizerState(&desc, &state)) ? state : 0;
	}
};

template <>
class StateCreator<ID3D11SamplerState>
{
public:
	static ID3D11SamplerState* Create(ID3D11Device* device, const D3D11_SAMPLER_DESC& desc)
	{
		ID3D11SamplerState* state;
		return SUCCEEDED(device->CreateSamplerState(&desc, &state)) ? state : 0;
	}
};

END_INANITY_DX

#endif
