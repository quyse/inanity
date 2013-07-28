#include "Dx11BlendState.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

ID3D11BlendState* Dx11BlendState::GetBlendStateInterface(ID3D11Device* deviceInterface)
{
	Update(deviceInterface);
	return blendState;
}

D3D11_BLEND Dx11BlendState::ConvertColorSource(ColorSource colorSource)
{
	D3D11_BLEND r;
	switch(colorSource)
	{
	case colorSourceZero:
		r = D3D11_BLEND_ZERO; break;
	case colorSourceOne:
		r = D3D11_BLEND_ONE; break;
	case colorSourceSrc:
		r = D3D11_BLEND_SRC_COLOR; break;
	case colorSourceInvSrc:
		r = D3D11_BLEND_INV_SRC_COLOR; break;
	case colorSourceSrcAlpha:
		r = D3D11_BLEND_SRC_ALPHA; break;
	case colorSourceInvSrcAlpha:
		r = D3D11_BLEND_INV_SRC_ALPHA; break;
	case colorSourceDest:
		r = D3D11_BLEND_DEST_COLOR; break;
	case colorSourceInvDest:
		r = D3D11_BLEND_INV_DEST_COLOR; break;
	case colorSourceDestAlpha:
		r = D3D11_BLEND_DEST_ALPHA; break;
	case colorSourceInvDestAlpha:
		r = D3D11_BLEND_INV_DEST_ALPHA; break;
	default:
		THROW("Unknown color source");
	}
	return r;
}

D3D11_BLEND Dx11BlendState::ConvertAlphaSource(AlphaSource alphaSource)
{
	D3D11_BLEND r;
	switch(alphaSource)
	{
	case alphaSourceZero:
		r = D3D11_BLEND_ZERO; break;
	case alphaSourceOne:
		r = D3D11_BLEND_ONE; break;
	case alphaSourceSrc:
		r = D3D11_BLEND_SRC_ALPHA; break;
	case alphaSourceInvSrc:
		r = D3D11_BLEND_INV_SRC_ALPHA; break;
	case alphaSourceDest:
		r = D3D11_BLEND_DEST_ALPHA; break;
	case alphaSourceInvDest:
		r = D3D11_BLEND_INV_DEST_ALPHA; break;
	default:
		THROW("Unknown alpha source");
	}
	return r;
}

D3D11_BLEND_OP Dx11BlendState::ConvertOperation(Operation operation)
{
	D3D11_BLEND_OP r;
	switch(operation)
	{
	case operationAdd:
		r = D3D11_BLEND_OP_ADD; break;
	case operationSubtractAB:
		r = D3D11_BLEND_OP_REV_SUBTRACT; break;
	case operationSubtractBA:
		r = D3D11_BLEND_OP_SUBTRACT; break;
	case operationMin:
		r = D3D11_BLEND_OP_MIN; break;
	case operationMax:
		r = D3D11_BLEND_OP_MAX; break;
	default:
		THROW("Unknown operation");
	}
	return r;
}

void Dx11BlendState::Update(ID3D11Device* deviceInterface)
{
	if(!dirty)
		return;

	// удалить предыдущий объект
	blendState = 0;

	try
	{
		D3D11_BLEND_DESC desc = CD3D11_BLEND_DESC(CD3D11_DEFAULT());

		// пока не реализовано
		desc.AlphaToCoverageEnable = FALSE;
		// пока смешивание общее для всех таргетов
		desc.IndependentBlendEnable = FALSE;
		
		D3D11_RENDER_TARGET_BLEND_DESC& d = desc.RenderTarget[0];

		d.BlendEnable = enable;
		d.SrcBlend = ConvertColorSource(sourceColor);
		d.DestBlend = ConvertColorSource(destColor);
		d.BlendOp = ConvertOperation(colorOperation);
		d.SrcBlendAlpha = ConvertAlphaSource(sourceAlpha);
		d.DestBlendAlpha = ConvertAlphaSource(destAlpha);
		d.BlendOpAlpha = ConvertOperation(alphaOperation);

		ID3D11BlendState* blendStateInterface;
		if(FAILED(deviceInterface->CreateBlendState(&desc, &blendStateInterface)))
			THROW("Can't create blend state");

		blendState = blendStateInterface;

		dirty = false;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't update DirectX blend state", exception);
	}
}

END_INANITY_GRAPHICS
