#include "DxBlendState.hpp"
#include "DxDevice.hpp"
#include "../Exception.hpp"

DxBlendState::DxBlendState(ptr<DxDevice> device) : device(device) {}

ID3D11BlendState* DxBlendState::GetBlendStateInterface()
{
	Update();
	return blendState;
}

D3D11_BLEND DxBlendState::ConvertColorSource(ColorSource colorSource)
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
		THROW_PRIMARY_EXCEPTION("Unknown color source");
	}
	return r;
}

D3D11_BLEND DxBlendState::ConvertAlphaSource(AlphaSource alphaSource)
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
		THROW_PRIMARY_EXCEPTION("Unknown alpha source");
	}
	return r;
}

D3D11_BLEND_OP DxBlendState::ConvertOperation(Operation operation)
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
		THROW_PRIMARY_EXCEPTION("Unknown operation");
	}
	return r;
}

void DxBlendState::Update()
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
		if(FAILED(device->GetDeviceInterface()->CreateBlendState(&desc, &blendStateInterface)))
			THROW_PRIMARY_EXCEPTION("Can't create blend state");

		blendState = blendStateInterface;

		dirty = false;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't update DirectX blend state", exception);
	}
}
