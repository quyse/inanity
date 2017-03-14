#include "Dx11BlendState.hpp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

D3D11_BLEND Dx11BlendState::ConvertColorSource(ColorSource colorSource)
{
	switch(colorSource)
	{
	case colorSourceZero:
		return D3D11_BLEND_ZERO;
	case colorSourceOne:
		return D3D11_BLEND_ONE;
	case colorSourceSrc:
		return D3D11_BLEND_SRC_COLOR;
	case colorSourceInvSrc:
		return D3D11_BLEND_INV_SRC_COLOR;
	case colorSourceSrcAlpha:
		return D3D11_BLEND_SRC_ALPHA;
	case colorSourceInvSrcAlpha:
		return D3D11_BLEND_INV_SRC_ALPHA;
	case colorSourceDest:
		return D3D11_BLEND_DEST_COLOR;
	case colorSourceInvDest:
		return D3D11_BLEND_INV_DEST_COLOR;
	case colorSourceDestAlpha:
		return D3D11_BLEND_DEST_ALPHA;
	case colorSourceInvDestAlpha:
		return D3D11_BLEND_INV_DEST_ALPHA;
	case colorSourceSecondSrc:
		return D3D11_BLEND_SRC1_COLOR;
	case colorSourceInvSecondSrc:
		return D3D11_BLEND_INV_SRC1_COLOR;
	case colorSourceSecondSrcAlpha:
		return D3D11_BLEND_SRC1_ALPHA;
	case colorSourceInvSecondSrcAlpha:
		return D3D11_BLEND_INV_SRC1_ALPHA;
	default:
		THROW("Unknown color source");
	}
}

D3D11_BLEND Dx11BlendState::ConvertAlphaSource(AlphaSource alphaSource)
{
	switch(alphaSource)
	{
	case alphaSourceZero:
		return D3D11_BLEND_ZERO;
	case alphaSourceOne:
		return D3D11_BLEND_ONE;
	case alphaSourceSrc:
		return D3D11_BLEND_SRC_ALPHA;
	case alphaSourceInvSrc:
		return D3D11_BLEND_INV_SRC_ALPHA;
	case alphaSourceDest:
		return D3D11_BLEND_DEST_ALPHA;
	case alphaSourceInvDest:
		return D3D11_BLEND_INV_DEST_ALPHA;
	case alphaSourceSecondSrc:
		return D3D11_BLEND_SRC1_ALPHA;
	case alphaSourceInvSecondSrc:
		return D3D11_BLEND_INV_SRC1_ALPHA;
	default:
		THROW("Unknown alpha source");
	}
}

D3D11_BLEND_OP Dx11BlendState::ConvertOperation(Operation operation)
{
	switch(operation)
	{
	case operationAdd:
		return D3D11_BLEND_OP_ADD;
	case operationSubtractAB:
		return D3D11_BLEND_OP_SUBTRACT;
	case operationSubtractBA:
		return D3D11_BLEND_OP_REV_SUBTRACT;
	case operationMin:
		return D3D11_BLEND_OP_MIN;
	case operationMax:
		return D3D11_BLEND_OP_MAX;
	default:
		THROW("Unknown operation");
	}
}

void Dx11BlendState::Update(ID3D11Device* deviceInterface)
{
	if(!dirty)
		return;

	// удалить предыдущий объект
	blendState = 0;

	BEGIN_TRY();

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

	if(FAILED(deviceInterface->CreateBlendState(&desc, &blendState)))
		THROW("Can't create blend state");

	dirty = false;

	END_TRY("Can't update DirectX blend state");
}

ID3D11BlendState* Dx11BlendState::GetBlendStateInterface(ID3D11Device* deviceInterface)
{
	Update(deviceInterface);
	return blendState;
}

END_INANITY_GRAPHICS
