#include "Dx11Context.hpp"
#include "Dx11System.hpp"
#include "Dx11Device.hpp"
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

int Dx11Context::GetRasterizerStateKey(const ContextState& contextState)
{
	int key = ((int)contextState.fillMode) | (((int)contextState.cullMode) << 1);
	// если состояние растеризатора отсутствует, создать его
	if(!rasterizerStateCache[key])
	{
		D3D11_RASTERIZER_DESC desc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT());

		switch(contextState.fillMode)
		{
		case ContextState::fillModeWireframe:
			desc.FillMode = D3D11_FILL_WIREFRAME;
			break;
		case ContextState::fillModeSolid:
			desc.FillMode = D3D11_FILL_SOLID;
			break;
		}

		switch(contextState.cullMode)
		{
		case ContextState::cullModeNone:
			desc.CullMode = D3D11_CULL_NONE;
			break;
		case ContextState::cullModeBack:
			desc.CullMode = D3D11_CULL_BACK;
			break;
		case ContextState::cullModeFront:
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

void Dx11Context::Update()
{
	// рендертаргеты
	{
		// определить, изменился ли хотя бы один рендертаргет
		bool dirty = false;
		if(forceReset || targetState.depthStencilBuffer != boundState.depthStencilBuffer)
			dirty = true;
		else
		{
			for(int i = 0; i < ContextState::renderTargetSlotsCount; ++i)
				if(targetState.renderBuffers[i] != boundState.renderBuffers[i])
				{
					dirty = true;
					break;
				}
		}

		if(dirty)
		{
			ID3D11RenderTargetView* views[ContextState::renderTargetSlotsCount];
			for(int i = 0; i < ContextState::renderTargetSlotsCount; ++i)
			{
				RenderBuffer* abstractRenderBuffer = targetState.renderBuffers[i];
				if(abstractRenderBuffer)
				{
					Dx11RenderBuffer* renderBuffer = fast_cast<Dx11RenderBuffer*>(abstractRenderBuffer);
					views[i] = renderBuffer->GetRenderTargetViewInterface();
				}
				else
					views[i] = 0;
			}

			ID3D11DepthStencilView* depthStencilView;
			DepthStencilBuffer* abstractDepthStencilBuffer = targetState.depthStencilBuffer;
			if(abstractDepthStencilBuffer)
			{
				Dx11DepthStencilBuffer* depthStencilBuffer = fast_cast<Dx11DepthStencilBuffer*>(abstractDepthStencilBuffer);
				depthStencilView = depthStencilBuffer->GetDepthStencilViewInterface();
			}
			else
				depthStencilView = 0;

			// выполнить вызов
			deviceContext->OMSetRenderTargets(ContextState::renderTargetSlotsCount, views, depthStencilView);

			// обновить актуальное состояние
			for(int i = 0; i < ContextState::renderTargetSlotsCount; ++i)
				boundState.renderBuffers[i] = targetState.renderBuffers[i];
			boundState.depthStencilBuffer = targetState.depthStencilBuffer;
		}
	}

	// текстуры
	{
		// сейчас единственная оптимизация - начало и конец в списке обновляемых текстур
		int end;
		if(forceReset)
			end = ContextState::textureSlotsCount;
		else
		{
			for(end = ContextState::textureSlotsCount - 1; end >= 0; --end)
				if(targetState.textures[end] != boundState.textures[end])
					break;
			++end;
		}
		int begin;
		if(forceReset)
			begin = 0;
		else
		{
			for(begin = 0; begin < end; ++begin)
				if(targetState.textures[begin] != boundState.textures[begin])
					break;
		}

		if(begin < end)
		{
			ID3D11ShaderResourceView* views[ContextState::textureSlotsCount];
			for(int i = begin; i < end; ++i)
			{
				Texture* abstractTexture = targetState.textures[i];
				if(abstractTexture)
				{
					Dx11Texture* texture = fast_cast<Dx11Texture*>(abstractTexture);
					views[i] = texture->GetShaderResourceViewInterface();
				}
				else
					views[i] = 0;
			}

			// установить текстуры
			deviceContext->VSSetShaderResources(begin, end - begin, views + begin);
			deviceContext->PSSetShaderResources(begin, end - begin, views + begin);

			// обновить актуальное состояние
			for(int i = begin; i < end; ++i)
				boundState.textures[i] = targetState.textures[i];
		}
	}

	// семплеры
	{
		// сейчас единственная оптимизация - начало и конец в списке обновляемых семплеров
		int end;
		if(forceReset)
			end = ContextState::textureSlotsCount;
		else
		{
			for(end = ContextState::textureSlotsCount - 1; end >= 0; --end)
				if(targetState.samplerStates[end] != boundState.samplerStates[end])
					break;
			++end;
		}
		int begin;
		if(forceReset)
			begin = 0;
		else
		{
			for(begin = 0; begin < end; ++begin)
				if(targetState.samplerStates[begin] != boundState.samplerStates[begin])
					break;
		}

		if(begin < end)
		{
			ID3D11SamplerState* states[ContextState::textureSlotsCount];
			for(int i = begin; i < end; ++i)
			{
				SamplerState* abstractSamplerState = targetState.samplerStates[i];
				if(abstractSamplerState)
				{
					Dx11SamplerState* samplerState = fast_cast<Dx11SamplerState*>(abstractSamplerState);
					states[i] = samplerState->GetSamplerStateInterface(device);
				}
				else
					states[i] = 0;
			}

			// установить семплеры
			deviceContext->VSSetSamplers(begin, end - begin, states + begin);
			deviceContext->PSSetSamplers(begin, end - begin, states + begin);

			// обновить актуальное состояние
			for(int i = begin; i < end; ++i)
				boundState.samplerStates[i] = targetState.samplerStates[i];
		}
	}

	// константные буферы
	{
		// сейчас единственная оптимизация - начало и конец в списке обновляемых константных буферов
		int end;
		if(forceReset)
			end = ContextState::uniformBufferSlotsCount;
		else
		{
			for(end = ContextState::uniformBufferSlotsCount - 1; end >= 0; --end)
				if(targetState.uniformBuffers[end] != boundState.uniformBuffers[end])
					break;
			++end;
		}
		int begin;
		if(forceReset)
			begin = 0;
		else
		{
			for(begin = 0; begin < end; ++begin)
				if(targetState.uniformBuffers[begin] != boundState.uniformBuffers[begin])
					break;
		}

		if(begin < end)
		{
			ID3D11Buffer* buffers[ContextState::uniformBufferSlotsCount];
			for(int i = begin; i < end; ++i)
			{
				UniformBuffer* abstractUniformBuffer = targetState.uniformBuffers[i];
				if(abstractUniformBuffer)
				{
					Dx11UniformBuffer* uniformBuffer = fast_cast<Dx11UniformBuffer*>(abstractUniformBuffer);
					buffers[i] = uniformBuffer->GetBufferInterface();
				}
				else
					buffers[i] = 0;
			}

			// установить буферы
			deviceContext->VSSetConstantBuffers(begin, end - begin, buffers + begin);
			deviceContext->PSSetConstantBuffers(begin, end - begin, buffers + begin);

			// обновить актуальное состояние
			for(int i = begin; i < end; ++i)
				boundState.uniformBuffers[i] = targetState.uniformBuffers[i];
		}
	}

	// разметка атрибутов
	if(forceReset || targetState.attributeBinding != boundState.attributeBinding || targetState.vertexShader != boundState.vertexShader)
	{
		deviceContext->IASetInputLayout(GetInputLayout(
			targetState.attributeBinding.FastCast<Dx11AttributeBinding>(),
			targetState.vertexShader.FastCast<Dx11VertexShader>()
		));

		boundState.attributeBinding = targetState.attributeBinding;
	}

	// вершинные буферы
	{
		// сейчас единственная оптимизация - начало и конец в списке обновляемых вершинных буферов
		int end;
		if(forceReset)
			end = ContextState::vertexBufferSlotsCount;
		else
		{
			for(end = ContextState::vertexBufferSlotsCount - 1; end >= 0; --end)
				if(targetState.vertexBuffers[end] != boundState.vertexBuffers[end])
					break;
			++end;
		}
		int begin;
		if(forceReset)
			begin = 0;
		else
		{
			for(begin = 0; begin < end; ++begin)
				if(targetState.vertexBuffers[begin] != boundState.vertexBuffers[begin])
					break;
		}

		if(begin < end)
		{
			ID3D11Buffer* buffers[ContextState::vertexBufferSlotsCount];
			UINT strides[ContextState::vertexBufferSlotsCount];
			UINT offsets[ContextState::vertexBufferSlotsCount];
			for(int i = begin; i < end; ++i)
			{
				VertexBuffer* abstractVertexBuffer = targetState.vertexBuffers[i];
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

			// установить буферы
			deviceContext->IASetVertexBuffers(begin, end - begin, buffers + begin, strides + begin, offsets + begin);
			// установить топологию геометрии
			deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			// обновить актуальное состояние
			for(int i = begin; i < end; ++i)
				boundState.vertexBuffers[i] = targetState.vertexBuffers[i];
		}
	}

	// индексный буфер
	if(forceReset || targetState.indexBuffer != boundState.indexBuffer)
	{
		IndexBuffer* abstractIndexBuffer = targetState.indexBuffer;
		if(abstractIndexBuffer)
		{
			Dx11IndexBuffer* indexBuffer = fast_cast<Dx11IndexBuffer*>(abstractIndexBuffer);
			deviceContext->IASetIndexBuffer(indexBuffer->GetBufferInterface(), indexBuffer->GetIndexFormat(), 0);
		}
		else
			deviceContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R16_UINT, 0);

		boundState.indexBuffer = targetState.indexBuffer;
	}

	// вершинный шейдер
	if(forceReset || targetState.vertexShader != boundState.vertexShader)
	{
		ID3D11VertexShader* shader;
		if(targetState.vertexShader)
			shader = fast_cast<Dx11VertexShader*>(&*targetState.vertexShader)->GetVertexShaderInterface();
		else
			shader = 0;
		deviceContext->VSSetShader(shader, NULL, 0);

		boundState.vertexShader = targetState.vertexShader;
	}

	// пиксельный шейдер
	if(forceReset || targetState.pixelShader != boundState.pixelShader)
	{
		ID3D11PixelShader* shader;
		if(targetState.pixelShader)
			shader = fast_cast<Dx11PixelShader*>(&*targetState.pixelShader)->GetPixelShaderInterface();
		else
			shader = 0;
		deviceContext->PSSetShader(shader, NULL, 0);

		boundState.pixelShader = targetState.pixelShader;
	}

	// состояние растеризатора (fill mode & cull mode)
	if(forceReset || targetState.fillMode != boundState.fillMode || targetState.cullMode != boundState.cullMode)
	{
		int key = GetRasterizerStateKey(targetState);
		deviceContext->RSSetState(rasterizerStateCache[key]);

		boundState.fillMode = targetState.fillMode;
		boundState.cullMode = targetState.cullMode;
	}

	// viewport
	if(forceReset || targetState.viewportWidth != boundState.viewportWidth || targetState.viewportHeight != boundState.viewportHeight)
	{
		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (float)targetState.viewportWidth;
		viewport.Height = (float)targetState.viewportHeight;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;
		deviceContext->RSSetViewports(1, &viewport);

		boundState.viewportWidth = targetState.viewportWidth;
		boundState.viewportHeight = targetState.viewportHeight;
	}

	// blend state
	if(forceReset || targetState.blendState != boundState.blendState)
	{
		ID3D11BlendState* blendState;
		if(targetState.blendState)
			blendState = fast_cast<Dx11BlendState*>(&*targetState.blendState)->GetBlendStateInterface(device);
		else
			blendState = 0;
		deviceContext->OMSetBlendState(blendState, 0, 0xffffffff);

		boundState.blendState = targetState.blendState;
	}

	forceReset = false;
}

void Dx11Context::ClearRenderBuffer(RenderBuffer* renderBuffer, const float* color)
{
	deviceContext->ClearRenderTargetView(
		fast_cast<Dx11RenderBuffer*>(renderBuffer)->GetRenderTargetViewInterface(),
		color);
}

void Dx11Context::ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, float depth)
{
	deviceContext->ClearDepthStencilView(
		fast_cast<Dx11DepthStencilBuffer*>(depthStencilBuffer)->GetDepthStencilViewInterface(),
		D3D11_CLEAR_DEPTH, depth, 0);
}

void Dx11Context::ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, unsigned stencil)
{
	deviceContext->ClearDepthStencilView(
		fast_cast<Dx11DepthStencilBuffer*>(depthStencilBuffer)->GetDepthStencilViewInterface(),
		D3D11_CLEAR_STENCIL, 0, stencil);
}

void Dx11Context::ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, float depth, unsigned stencil)
{
	deviceContext->ClearDepthStencilView(
		fast_cast<Dx11DepthStencilBuffer*>(depthStencilBuffer)->GetDepthStencilViewInterface(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}

void Dx11Context::SetBufferData(ID3D11Resource* resource, const void* data, int size, int bufferSize)
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

void Dx11Context::SetUniformBufferData(UniformBuffer* abstractUniformBuffer, const void* data, int size)
{
	Dx11UniformBuffer* uniformBuffer = fast_cast<Dx11UniformBuffer*>(abstractUniformBuffer);

	SetBufferData(uniformBuffer->GetBufferInterface(), data, size, uniformBuffer->GetSize());
}

void Dx11Context::SetVertexBufferData(VertexBuffer* abstractVertexBuffer, const void* data, int size)
{
	Dx11VertexBuffer* vertexBuffer = fast_cast<Dx11VertexBuffer*>(abstractVertexBuffer);

	SetBufferData(vertexBuffer->GetBufferInterface(), data, size, vertexBuffer->GetSize());
}

void Dx11Context::Draw(int count)
{
	Update();

	if(boundState.indexBuffer)
		deviceContext->DrawIndexed(count >= 0 ? count : fast_cast<Dx11IndexBuffer*>(&*boundState.indexBuffer)->GetIndicesCount(), 0, 0);
	else
		deviceContext->Draw(count >= 0 ? count : fast_cast<Dx11VertexBuffer*>(&*boundState.vertexBuffers[0])->GetVerticesCount(), 0);
}

void Dx11Context::DrawInstanced(int instancesCount, int count)
{
	Update();

	if(boundState.indexBuffer)
		deviceContext->DrawIndexedInstanced(count >= 0 ? count : fast_cast<Dx11IndexBuffer*>(&*boundState.indexBuffer)->GetIndicesCount(), instancesCount, 0, 0, 0);
	else
		deviceContext->DrawInstanced(count >= 0 ? count : fast_cast<Dx11VertexBuffer*>(&*boundState.vertexBuffers[0])->GetVerticesCount(), instancesCount, 0, 0);
}

END_INANITY_GRAPHICS
