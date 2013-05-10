#include "DxContext.hpp"
#include "DxSystem.hpp"
#include "DxDevice.hpp"
#include "DxRenderBuffer.hpp"
#include "DxDepthStencilBuffer.hpp"
#include "DxTexture.hpp"
#include "DxSamplerState.hpp"
#include "DxUniformBuffer.hpp"
#include "DxVertexShader.hpp"
#include "DxPixelShader.hpp"
#include "DxAttributeBinding.hpp"
#include "DxVertexBuffer.hpp"
#include "DxIndexBuffer.hpp"
#include "VertexLayout.hpp"
#include "DxBlendState.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

size_t std::hash<Inanity::Graphics::DxInputLayoutCacheKey>::operator()
	(const Inanity::Graphics::DxInputLayoutCacheKey& key) const
{
	return (size_t)&*key.vertexShader ^ (size_t)&*key.attributeBinding;
}

BEGIN_INANITY_GRAPHICS

DxInputLayoutCacheKey::DxInputLayoutCacheKey(ptr<DxAttributeBinding> attributeBinding, ptr<DxVertexShader> vertexShader)
: attributeBinding(attributeBinding), vertexShader(vertexShader) {}

bool operator==(const DxInputLayoutCacheKey& a, const DxInputLayoutCacheKey& b)
{
	return a.attributeBinding == b.attributeBinding && a.vertexShader == b.vertexShader;
}

DxContext::DxContext(ComPointer<ID3D11Device> device, ComPointer<ID3D11DeviceContext> deviceContext)
: device(device), deviceContext(deviceContext) {}

ID3D11InputLayout* DxContext::GetInputLayout(ptr<DxAttributeBinding> attributeBinding, ptr<DxVertexShader> vertexShader)
{
	DxInputLayoutCacheKey key(attributeBinding, vertexShader);
	InputLayoutCache::const_iterator i = inputLayoutCache.find(key);
	if(i != inputLayoutCache.end())
		return i->second;

	ComPointer<ID3D11InputLayout> inputLayout = attributeBinding->CreateInputLayout(device, vertexShader);
	inputLayoutCache[key] = inputLayout;

	return inputLayout;
}

int DxContext::GetRasterizerStateKey(const ContextState& contextState)
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

		ID3D11RasterizerState* rasterizerStateInterface;
		if(FAILED(device->CreateRasterizerState(&desc, &rasterizerStateInterface)))
			THROW_PRIMARY_EXCEPTION("Can't create rasterizer state");
		rasterizerStateCache[key] = rasterizerStateInterface;
	}

	return key;
}

void DxContext::Update()
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
					DxRenderBuffer* renderBuffer = fast_cast<DxRenderBuffer*>(abstractRenderBuffer);
					views[i] = renderBuffer->GetRenderTargetViewInterface();
				}
				else
					views[i] = 0;
			}

			ID3D11DepthStencilView* depthStencilView;
			DepthStencilBuffer* abstractDepthStencilBuffer = targetState.depthStencilBuffer;
			if(abstractDepthStencilBuffer)
			{
				DxDepthStencilBuffer* depthStencilBuffer = fast_cast<DxDepthStencilBuffer*>(abstractDepthStencilBuffer);
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
					DxTexture* texture = fast_cast<DxTexture*>(abstractTexture);
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
					DxSamplerState* samplerState = fast_cast<DxSamplerState*>(abstractSamplerState);
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
					DxUniformBuffer* uniformBuffer = fast_cast<DxUniformBuffer*>(abstractUniformBuffer);
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
			targetState.attributeBinding.FastCast<DxAttributeBinding>(),
			targetState.vertexShader.FastCast<DxVertexShader>()
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
					DxVertexBuffer* vertexBuffer = fast_cast<DxVertexBuffer*>(abstractVertexBuffer);
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
			DxIndexBuffer* indexBuffer = fast_cast<DxIndexBuffer*>(abstractIndexBuffer);
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
			shader = fast_cast<DxVertexShader*>(&*targetState.vertexShader)->GetVertexShaderInterface();
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
			shader = fast_cast<DxPixelShader*>(&*targetState.pixelShader)->GetPixelShaderInterface();
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
			blendState = fast_cast<DxBlendState*>(&*targetState.blendState)->GetBlendStateInterface(device);
		else
			blendState = 0;
		deviceContext->OMSetBlendState(blendState, 0, 0xffffffff);

		boundState.blendState = targetState.blendState;
	}

	forceReset = false;
}

void DxContext::ClearRenderBuffer(RenderBuffer* renderBuffer, const float* color)
{
	deviceContext->ClearRenderTargetView(
		fast_cast<DxRenderBuffer*>(renderBuffer)->GetRenderTargetViewInterface(),
		color);
}

void DxContext::ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, float depth)
{
	deviceContext->ClearDepthStencilView(
		fast_cast<DxDepthStencilBuffer*>(depthStencilBuffer)->GetDepthStencilViewInterface(),
		D3D11_CLEAR_DEPTH, depth, 0);
}

void DxContext::ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, unsigned stencil)
{
	deviceContext->ClearDepthStencilView(
		fast_cast<DxDepthStencilBuffer*>(depthStencilBuffer)->GetDepthStencilViewInterface(),
		D3D11_CLEAR_STENCIL, 0, stencil);
}

void DxContext::ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, float depth, unsigned stencil)
{
	deviceContext->ClearDepthStencilView(
		fast_cast<DxDepthStencilBuffer*>(depthStencilBuffer)->GetDepthStencilViewInterface(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}

void DxContext::SetUniformBufferData(UniformBuffer* abstractUniformBuffer, const void* data, size_t size)
{
	// проверить, что размер не больше, чем нужно
	if(size > abstractUniformBuffer->GetSize())
		THROW_PRIMARY_EXCEPTION("Data size to set into uniform buffer is too big");

	DxUniformBuffer* uniformBuffer = fast_cast<DxUniformBuffer*>(abstractUniformBuffer);

	//спроецировать буфер
	D3D11_MAPPED_SUBRESOURCE bufferData;
	ID3D11Resource* resource = uniformBuffer->GetBufferInterface();
	if(FAILED(deviceContext->Map(resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData)))
		THROW_PRIMARY_EXCEPTION("Can't map graphics buffer");

	//скопировать данные
	memcpy(bufferData.pData, data, size);

	//отключить проекцию буфера
	deviceContext->Unmap(resource, 0);
}

void DxContext::Draw()
{
	Update();

	if(boundState.indexBuffer)
		deviceContext->DrawIndexed(fast_cast<DxIndexBuffer*>(&*boundState.indexBuffer)->GetIndicesCount(), 0, 0);
	else
		deviceContext->Draw(fast_cast<DxVertexBuffer*>(&*boundState.vertexBuffers[0])->GetVerticesCount(), 0);
}

void DxContext::DrawInstanced(int instancesCount)
{
	Update();

	if(boundState.indexBuffer)
		deviceContext->DrawIndexedInstanced(fast_cast<DxIndexBuffer*>(&*boundState.indexBuffer)->GetIndicesCount(), instancesCount, 0, 0, 0);
	else
		deviceContext->DrawInstanced(fast_cast<DxVertexBuffer*>(&*boundState.vertexBuffers[0])->GetVerticesCount(), instancesCount, 0, 0);
}

END_INANITY_GRAPHICS
