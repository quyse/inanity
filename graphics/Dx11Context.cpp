#include "Dx11Context.hpp"
#include "Dx11System.hpp"
#include "Dx11Device.hpp"
#include "Dx11FrameBuffer.hpp"
#include "Dx11RenderBuffer.hpp"
#include "Dx11DepthStencilBuffer.hpp"
#include "Dx11Texture.hpp"
#include "Dx11SamplerState.hpp"
#include "Dx11UniformBuffer.hpp"
#include "Dx11VertexShader.hpp"
#include "Dx11PixelShader.hpp"
#include "Dx11AttributeBinding.hpp"
#include "Dx11VertexBuffer.hpp"
#include "Dx11IndexBuffer.hpp"
#include "VertexLayout.hpp"
#include "Dx11BlendState.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

size_t std::hash<Inanity::Graphics::Dx11InputLayoutCacheKey>::operator()
	(const Inanity::Graphics::Dx11InputLayoutCacheKey& key) const
{
	return (size_t)&*key.vertexShader ^ (size_t)&*key.attributeBinding;
}

BEGIN_INANITY_GRAPHICS

Dx11InputLayoutCacheKey::Dx11InputLayoutCacheKey(ptr<Dx11AttributeBinding> attributeBinding, ptr<Dx11VertexShader> vertexShader)
: attributeBinding(attributeBinding), vertexShader(vertexShader) {}

bool operator==(const Dx11InputLayoutCacheKey& a, const Dx11InputLayoutCacheKey& b)
{
	return a.attributeBinding == b.attributeBinding && a.vertexShader == b.vertexShader;
}

Dx11Context::Dx11Context(ptr<Dx11Device> deviceObject) :
	deviceObject(deviceObject),
	device(deviceObject->GetDeviceInterface()),
	deviceContext(deviceObject->GetDeviceContextInterface())
{}

ID3D11InputLayout* Dx11Context::GetInputLayout(ptr<Dx11AttributeBinding> attributeBinding, ptr<Dx11VertexShader> vertexShader)
{
	Dx11InputLayoutCacheKey key(attributeBinding, vertexShader);
	InputLayoutCache::const_iterator i = inputLayoutCache.find(key);
	if(i != inputLayoutCache.end())
		return i->second;

	ComPointer<ID3D11InputLayout> inputLayout = attributeBinding->CreateInputLayout(device, vertexShader);
	inputLayoutCache[key] = inputLayout;

	return inputLayout;
}

int Dx11Context::GetRasterizerStateKey()
{
	FillMode fillMode = ((LetFillMode*)cellFillMode.top)->fillMode;
	CullMode cullMode = ((LetCullMode*)cellCullMode.top)->cullMode;
	int key = ((int)fillMode) | (((int)cullMode) << 1);

	// if rasterizer state missed, create new
	if(!rasterizerStateCache[key])
	{
		D3D11_RASTERIZER_DESC desc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT());

		switch(fillMode)
		{
		case fillModeWireframe:
			desc.FillMode = D3D11_FILL_WIREFRAME;
			break;
		case fillModeSolid:
			desc.FillMode = D3D11_FILL_SOLID;
			break;
		}

		switch(cullMode)
		{
		case cullModeNone:
			desc.CullMode = D3D11_CULL_NONE;
			break;
		case cullModeBack:
			desc.CullMode = D3D11_CULL_BACK;
			break;
		case cullModeFront:
			desc.CullMode = D3D11_CULL_FRONT;
			break;
		}

		ComPointer<ID3D11RasterizerState> rasterizerState;
		if(FAILED(device->CreateRasterizerState(&desc, &rasterizerState)))
			THROW("Can't create rasterizer state");
		rasterizerStateCache[key] = rasterizerState;
	}

	return key;
}

int Dx11Context::GetDepthStencilStateKey()
{
	DepthTestFunc depthTestFunc = ((LetDepthTestFunc*)cellDepthTestFunc.top)->depthTestFunc;
	bool depthWrite = ((LetDepthWrite*)cellDepthWrite.top)->depthWrite;
	int key = ((int)depthTestFunc) | (((int)depthWrite) << 3);

	// if depth-stencil state missed, create new
	if(!depthStencilStateCache[key])
	{
		D3D11_DEPTH_STENCIL_DESC desc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT());

		switch(depthTestFunc)
		{
		case depthTestFuncNever:
			desc.DepthFunc = D3D11_COMPARISON_NEVER;
			break;
		case depthTestFuncLess:
			desc.DepthFunc = D3D11_COMPARISON_LESS;
			break;
		case depthTestFuncLessOrEqual:
			desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
			break;
		case depthTestFuncEqual:
			desc.DepthFunc = D3D11_COMPARISON_EQUAL;
			break;
		case depthTestFuncNonEqual:
			desc.DepthFunc = D3D11_COMPARISON_NOT_EQUAL;
			break;
		case depthTestFuncGreaterOrEqual:
			desc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
			break;
		case depthTestFuncGreater:
			desc.DepthFunc = D3D11_COMPARISON_GREATER;
			break;
		case depthTestFuncAlways:
			desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
			break;
		}

		desc.DepthEnable = depthTestFunc != depthTestFuncAlways || depthWrite;
		desc.DepthWriteMask = depthWrite ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;

		ComPointer<ID3D11DepthStencilState> depthStencilState;
		if(FAILED(device->CreateDepthStencilState(&desc, &depthStencilState)))
			THROW("Can't create depth-stencil state");
		depthStencilStateCache[key] = depthStencilState;
	}

	return key;
}

void Dx11Context::Update()
{
	// framebuffer
	THROW_ASSERT(cellFrameBuffer.top);
	if(!cellFrameBuffer.IsActual())
	{
		LetFrameBuffer* top = (LetFrameBuffer*)cellFrameBuffer.top;
		LetFrameBuffer* current = (LetFrameBuffer*)cellFrameBuffer.current;

		THROW_ASSERT(top->frameBuffer);

		Dx11FrameBuffer* frameBuffer = fast_cast<Dx11FrameBuffer*>(&*top->frameBuffer);
		if(!current || current->frameBuffer != top->frameBuffer || frameBuffer->IsDirty())
			frameBuffer->Apply(deviceContext);

		cellFrameBuffer.Actual();
	}

	// samplers
	{
		// the only optimization for now is begin and end
		int end;
		for(end = samplersCount - 1; end >= 0; --end)
			if(!cellSamplers[end].IsActual())
				break;
		++end;
		int begin;
		for(begin = 0; begin < end; ++begin)
			if(!cellSamplers[begin].IsActual())
				break;

		if(begin < end)
		{
			ID3D11ShaderResourceView* views[samplersCount];
			ID3D11SamplerState* states[samplersCount];
			for(int i = begin; i < end; ++i)
			{
				const LetSampler* let = (LetSampler*)cellSamplers[i].top;
				Texture* abstractTexture = let ? let->texture : 0;
				views[i] = abstractTexture
					? fast_cast<Dx11Texture*>(abstractTexture)->GetShaderResourceViewInterface()
					: 0;
				SamplerState* abstractSamplerState = let ? let->samplerState : 0;
				states[i] = abstractSamplerState
					? fast_cast<Dx11SamplerState*>(abstractSamplerState)->GetSamplerStateInterface(device)
					: 0;
			}

			// set textures
			deviceContext->VSSetShaderResources(begin, end - begin, views + begin);
			deviceContext->PSSetShaderResources(begin, end - begin, views + begin);
			// set samplers
			deviceContext->VSSetSamplers(begin, end - begin, states + begin);
			deviceContext->PSSetSamplers(begin, end - begin, states + begin);

			// update actual state
			for(int i = begin; i < end; ++i)
				cellSamplers[i].Actual();
		}
	}

	// uniform buffers
	{
		// the only optimization for now is begin and end
		int end;
		for(end = uniformBuffersCount - 1; end >= 0; --end)
			if(!cellUniformBuffers[end].IsActual())
				break;
		++end;
		int begin;
		for(begin = 0; begin < end; ++begin)
			if(!cellUniformBuffers[begin].IsActual())
				break;

		if(begin < end)
		{
			ID3D11Buffer* buffers[uniformBuffersCount];
			for(int i = begin; i < end; ++i)
			{
				LetUniformBuffer* let = (LetUniformBuffer*)cellUniformBuffers[i].top;
				UniformBuffer* abstractUniformBuffer = let ? let->uniformBuffer : 0;
				buffers[i] = abstractUniformBuffer
					? fast_cast<Dx11UniformBuffer*>(abstractUniformBuffer)->GetBufferInterface()
					: 0;
			}

			// set buffers
			deviceContext->VSSetConstantBuffers(begin, end - begin, buffers + begin);
			deviceContext->PSSetConstantBuffers(begin, end - begin, buffers + begin);

			// update actual state
			for(int i = begin; i < end; ++i)
				cellUniformBuffers[i].Actual();
		}
	}

	// input layout (depends on attribute binding and vertex shader)
	THROW_ASSERT(cellAttributeBinding.top);
	THROW_ASSERT(cellVertexShader.top);
	if(!cellAttributeBinding.IsActual() || !cellVertexShader.IsActual())
	{
		deviceContext->IASetInputLayout(GetInputLayout(
			((LetAttributeBinding*)cellAttributeBinding.top)->attributeBinding.FastCast<Dx11AttributeBinding>(),
			((LetVertexShader*)cellVertexShader.top)->vertexShader.FastCast<Dx11VertexShader>()
		));

		cellAttributeBinding.Actual();
	}

	// vertex buffers
	{
		// the only optimization for now is begin and end
		int end;
		for(end = vertexBuffersCount - 1; end >= 0; --end)
			if(!cellVertexBuffers[end].IsActual())
				break;
		++end;
		int begin;
		for(begin = 0; begin < end; ++begin)
			if(!cellVertexBuffers[begin].IsActual())
				break;

		if(begin < end)
		{
			ID3D11Buffer* buffers[vertexBuffersCount];
			UINT strides[vertexBuffersCount];
			UINT offsets[vertexBuffersCount];
			for(int i = begin; i < end; ++i)
			{
				LetVertexBuffer* let = (LetVertexBuffer*)cellVertexBuffers[i].top;
				VertexBuffer* abstractVertexBuffer = let ? let->vertexBuffer : 0;
				if(abstractVertexBuffer)
				{
					Dx11VertexBuffer* vertexBuffer = fast_cast<Dx11VertexBuffer*>(abstractVertexBuffer);
					buffers[i] = vertexBuffer->GetBufferInterface();
					strides[i] = vertexBuffer->GetLayout()->GetStride();
					offsets[i] = 0;
				}
				else
				{
					buffers[i] = 0;
					strides[i] = 0;
					offsets[i] = 0;
				}
			}

			// set buffers and primitive topology
			deviceContext->IASetVertexBuffers(begin, end - begin, buffers + begin, strides + begin, offsets + begin);
			deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			// update actual state
			for(int i = begin; i < end; ++i)
				cellVertexBuffers[i].Actual();
		}
	}

	// index buffer
	if(!cellIndexBuffer.IsActual())
	{
		LetIndexBuffer* let = (LetIndexBuffer*)cellIndexBuffer.top;
		IndexBuffer* abstractIndexBuffer = let->indexBuffer;
		if(abstractIndexBuffer)
		{
			Dx11IndexBuffer* indexBuffer = fast_cast<Dx11IndexBuffer*>(abstractIndexBuffer);
			deviceContext->IASetIndexBuffer(indexBuffer->GetBufferInterface(), indexBuffer->GetIndexFormat(), 0);
		}
		else
			deviceContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R16_UINT, 0);

		cellIndexBuffer.Actual();
	}

	// vertex shader
	if(!cellVertexShader.IsActual())
	{
		LetVertexShader* let = (LetVertexShader*)cellVertexShader.top;
		VertexShader* abstractVertexShader = let->vertexShader;
		ID3D11VertexShader* shader = abstractVertexShader
			? fast_cast<Dx11VertexShader*>(abstractVertexShader)->GetVertexShaderInterface()
			: 0;
		deviceContext->VSSetShader(shader, NULL, 0);

		cellVertexShader.Actual();
	}

	// pixel shader
	if(!cellPixelShader.IsActual())
	{
		LetPixelShader* let = (LetPixelShader*)cellPixelShader.top;
		PixelShader* abstractPixelShader = let->pixelShader;
		ID3D11PixelShader* shader = abstractPixelShader
			? fast_cast<Dx11PixelShader*>(abstractPixelShader)->GetPixelShaderInterface()
			: 0;
		deviceContext->PSSetShader(shader, NULL, 0);

		cellPixelShader.Actual();
	}

	// rasterizer state (fill mode & cull mode)
	if(!cellFillMode.IsActual() || !cellCullMode.IsActual())
	{
		int key = GetRasterizerStateKey();
		deviceContext->RSSetState(rasterizerStateCache[key]);

		cellFillMode.Actual();
		cellCullMode.Actual();
	}

	// depth-stencil state (depth test func & depth write)
	if(!cellDepthTestFunc.IsActual() || !cellDepthWrite.IsActual())
	{
		int key = GetDepthStencilStateKey();
		deviceContext->OMSetDepthStencilState(depthStencilStateCache[key], 0);

		cellDepthTestFunc.Actual();
		cellDepthWrite.Actual();
	}

	// viewport
	THROW_ASSERT(cellViewport.top);
	if(!cellViewport.IsActual())
	{
		LetViewport* let = (LetViewport*)cellViewport.top;
		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (float)let->viewportWidth;
		viewport.Height = (float)let->viewportHeight;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;
		deviceContext->RSSetViewports(1, &viewport);

		cellViewport.Actual();
	}

	// blend state
	if(!cellBlendState.IsActual())
	{
		LetBlendState* let = (LetBlendState*)cellBlendState.top;
		BlendState* abstractBlendState = let->blendState;
		ID3D11BlendState* blendState = abstractBlendState
			? fast_cast<Dx11BlendState*>(abstractBlendState)->GetBlendStateInterface(device)
			: 0;
		deviceContext->OMSetBlendState(blendState, 0, 0xffffffff);

		cellBlendState.Actual();
	}
}

void Dx11Context::ClearColor(int colorBufferIndex, const float* color)
{
	LetFrameBuffer* let = (LetFrameBuffer*)cellFrameBuffer.top;
	THROW_ASSERT(let);
	THROW_ASSERT(let->frameBuffer);
	deviceContext->ClearRenderTargetView(
		fast_cast<Dx11FrameBuffer*>((FrameBuffer*)let->frameBuffer)
			->GetColorBuffer(colorBufferIndex)
				->GetRenderTargetViewInterface(),
		color);
}

void Dx11Context::ClearDepth(float depth)
{
	LetFrameBuffer* let = (LetFrameBuffer*)cellFrameBuffer.top;
	THROW_ASSERT(let);
	THROW_ASSERT(let->frameBuffer);
	deviceContext->ClearDepthStencilView(
		fast_cast<Dx11DepthStencilBuffer*>(
			&*let->frameBuffer->GetDepthStencilBuffer()
		)->GetDepthStencilViewInterface(),
		D3D11_CLEAR_DEPTH, depth, 0);
}

void Dx11Context::ClearStencil(unsigned stencil)
{
	LetFrameBuffer* let = (LetFrameBuffer*)cellFrameBuffer.top;
	THROW_ASSERT(let);
	THROW_ASSERT(let->frameBuffer);
	deviceContext->ClearDepthStencilView(
		fast_cast<Dx11DepthStencilBuffer*>(
			&*let->frameBuffer->GetDepthStencilBuffer()
		)->GetDepthStencilViewInterface(),
		D3D11_CLEAR_STENCIL, 0, stencil);
}

void Dx11Context::ClearDepthStencil(float depth, unsigned stencil)
{
	LetFrameBuffer* let = (LetFrameBuffer*)cellFrameBuffer.top;
	THROW_ASSERT(let);
	THROW_ASSERT(let->frameBuffer);
	deviceContext->ClearDepthStencilView(
		fast_cast<Dx11DepthStencilBuffer*>(
			&*let->frameBuffer->GetDepthStencilBuffer()
		)->GetDepthStencilViewInterface(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}

void Dx11Context::UploadBufferData(ID3D11Resource* resource, const void* data, int size, int bufferSize)
{
	// проверить, что размер не больше, чем нужно
	if(size > bufferSize)
		THROW("Size of data to set into DX11 buffer is too big");

	//спроецировать буфер
	D3D11_MAPPED_SUBRESOURCE bufferData;
	if(FAILED(deviceContext->Map(resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData)))
		THROW("Can't map DX11 buffer");

	//скопировать данные
	memcpy(bufferData.pData, data, size);

	//отключить проекцию буфера
	deviceContext->Unmap(resource, 0);
}

void Dx11Context::UploadUniformBufferData(UniformBuffer* abstractUniformBuffer, const void* data, int size)
{
	Dx11UniformBuffer* uniformBuffer = fast_cast<Dx11UniformBuffer*>(abstractUniformBuffer);

	UploadBufferData(uniformBuffer->GetBufferInterface(), data, size, uniformBuffer->GetSize());
}

void Dx11Context::UploadVertexBufferData(VertexBuffer* abstractVertexBuffer, const void* data, int size)
{
	Dx11VertexBuffer* vertexBuffer = fast_cast<Dx11VertexBuffer*>(abstractVertexBuffer);

	UploadBufferData(vertexBuffer->GetBufferInterface(), data, size, vertexBuffer->GetSize());
}

void Dx11Context::Draw(int count)
{
	Update();

	LetIndexBuffer* letIndexBuffer = (LetIndexBuffer*)cellIndexBuffer.current;
	IndexBuffer* abstractIndexBuffer = letIndexBuffer ? letIndexBuffer->indexBuffer : 0;
	if(abstractIndexBuffer)
		deviceContext->DrawIndexed(count >= 0 ? count : fast_cast<Dx11IndexBuffer*>(abstractIndexBuffer)->GetIndicesCount(), 0, 0);
	else
	{
		THROW_ASSERT(cellVertexBuffers[0].current);
		VertexBuffer* abstractVertexBuffer = ((LetVertexBuffer*)cellVertexBuffers[0].current)->vertexBuffer;
		THROW_ASSERT(abstractVertexBuffer);
		deviceContext->Draw(count >= 0 ? count : fast_cast<Dx11VertexBuffer*>(abstractVertexBuffer)->GetVerticesCount(), 0);
	}
}

void Dx11Context::DrawInstanced(int instancesCount, int count)
{
	Update();

	LetIndexBuffer* letIndexBuffer = (LetIndexBuffer*)cellIndexBuffer.current;
	IndexBuffer* abstractIndexBuffer = letIndexBuffer ? letIndexBuffer->indexBuffer : 0;
	if(abstractIndexBuffer)
		deviceContext->DrawIndexedInstanced(count >= 0 ? count : fast_cast<Dx11IndexBuffer*>(abstractIndexBuffer)->GetIndicesCount(), instancesCount, 0, 0, 0);
	else
	{
		THROW_ASSERT(cellVertexBuffers[0].current);
		VertexBuffer* abstractVertexBuffer = ((LetVertexBuffer*)cellVertexBuffers[0].current)->vertexBuffer;
		THROW_ASSERT(abstractVertexBuffer);
		deviceContext->DrawInstanced(count >= 0 ? count : fast_cast<Dx11VertexBuffer*>(abstractVertexBuffer)->GetVerticesCount(), instancesCount, 0, 0);
	}
}

END_INANITY_GRAPHICS
