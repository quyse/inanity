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
#include "DxVertexBuffer.hpp"
#include "DxInternalInputLayout.hpp"
#include "DxInternalInputLayoutCache.hpp"
#include "DxIndexBuffer.hpp"
#include "DxBlendState.hpp"
#include "Layout.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

DxContext::DxContext(ComPointer<ID3D11Device> device, ComPointer<ID3D11DeviceContext> deviceContext)
: device(device), deviceContext(deviceContext)
{
	// создать кэш входных разметок
	inputLayoutCache = NEW(DxInternalInputLayoutCache(this));
}

ptr<DxInternalInputLayout> DxContext::CreateInternalInputLayout(Layout* vertexLayout, DxVertexShader* vertexShader)
{
	try
	{
		// получить элементы разметки
		const std::vector<Layout::Element>& elements = vertexLayout->GetElements();
		if(elements.empty())
			THROW_PRIMARY_EXCEPTION("Vertex layout is empty");

		// структуры описания
		// количество не указывается, так как матрицы требуют несколько элементов
		std::vector<D3D11_INPUT_ELEMENT_DESC> descs;
		// строки для имён семантик, их количество известно
		std::vector<String> semanticNames(elements.size());

		// и заполнить их
		for(size_t i = 0; i < elements.size(); ++i)
		{
			const Layout::Element& element = elements[i];

			// получить количество необходимых описаний для элемента
			int needDescsCount;
			switch(element.dataType)
			{
			case DataTypes::Float4x4:
				needDescsCount = 4;
				break;
			default:
				needDescsCount = 1;
				break;
			}

			// выбрать формат элемента
			DXGI_FORMAT format;
			switch(element.dataType)
			{
			case DataTypes::Float:
				format = DXGI_FORMAT_R32_FLOAT;
				break;
			case DataTypes::Float2:
				format = DXGI_FORMAT_R32G32_FLOAT;
				break;
			case DataTypes::Float3:
				format = DXGI_FORMAT_R32G32B32_FLOAT;
				break;
			case DataTypes::Float4:
				format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				break;
			case DataTypes::Float4x4:
				format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				break;
			case DataTypes::UInt:
				format = DXGI_FORMAT_R32_UINT;
				break;
			case DataTypes::UInt2:
				format = DXGI_FORMAT_R32G32_UINT;
				break;
			case DataTypes::UInt3:
				format = DXGI_FORMAT_R32G32B32_UINT;
				break;
			case DataTypes::UInt4:
				format = DXGI_FORMAT_R32G32B32A32_UINT;
				break;
			default:
				THROW_PRIMARY_EXCEPTION("Unknown element type");
			}

			// размер элемента - пока всегда один вектор (float4 или uint4),
			// так как он нужен только для матриц float4x4
			const int elementSize = 16;

			// имя семантики получается переводом кода семантики в 26-ричную систему (буквы)
			String& semanticName = semanticNames[i];
			semanticName = DxSystem::GetSemanticString(element.semantic);

			// цикл по вот этим требуемым элементам
			for(int descIndex = 0; descIndex < needDescsCount; ++descIndex)
			{
				descs.push_back(D3D11_INPUT_ELEMENT_DESC());
				D3D11_INPUT_ELEMENT_DESC& desc = descs.back();

				// SemanticName в структуре будет заполнено позже,
				// потому что сейчас адреса в векторе ещё могут поменяться

				desc.SemanticName = semanticName.c_str();
				desc.SemanticIndex = descIndex;
				desc.Format = format;
				desc.InputSlot = 0;
				desc.AlignedByteOffset = element.offset + descIndex * elementSize;
				desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				desc.InstanceDataStepRate = 0;
			}
		}

		// получить код шейдера
		ptr<File> vertexShaderCode = vertexShader->GetCode();

		// создать разметку
		ID3D11InputLayout* inputLayoutInterface;
		if(FAILED(device->CreateInputLayout(&*descs.begin(), (UINT)descs.size(), vertexShaderCode->GetData(), vertexShaderCode->GetSize(), &inputLayoutInterface)))
			THROW_PRIMARY_EXCEPTION("Can't create input layout");

		return NEW(DxInternalInputLayout(inputLayoutInterface));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create DX internal input layout", exception);
	}
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
					states[i] = samplerState->GetSamplerStateInterface();
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

	// входная разметка
	if(forceReset || targetState.vertexBuffer != boundState.vertexBuffer || targetState.vertexShader != boundState.vertexShader)
	{
		ptr<DxInternalInputLayout> inputLayout = inputLayoutCache->GetInputLayout(targetState.vertexBuffer->GetLayout(), fast_cast<DxVertexShader*>(&*targetState.vertexShader));
		if(forceReset || inputLayout != boundInputLayout)
		{
			deviceContext->IASetInputLayout(inputLayout->GetInputLayoutInterface());
			boundInputLayout = inputLayout;
		}
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

	// вершинный буфер
	if(forceReset || targetState.vertexBuffer != boundState.vertexBuffer)
	{
		ID3D11Buffer* buffer = fast_cast<DxVertexBuffer*>(&*targetState.vertexBuffer)->GetBufferInterface();
		UINT stride = targetState.vertexBuffer->GetLayout()->GetStride();
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		boundState.vertexBuffer = targetState.vertexBuffer;
	}

	// индексный буфер
	if(forceReset || targetState.indexBuffer != boundState.indexBuffer)
	{
		ID3D11Buffer* buffer;
		DXGI_FORMAT format;
		if(targetState.indexBuffer)
		{
			buffer = fast_cast<DxIndexBuffer*>(&*targetState.indexBuffer)->GetBufferInterface();
			format = targetState.indexBuffer->GetIndexSize() == 4 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
		}
		else
		{
			buffer = 0;
			format = DXGI_FORMAT_R16_UINT;
		}
		deviceContext->IASetIndexBuffer(buffer, format, 0);

		boundState.indexBuffer = targetState.indexBuffer;
	}

	// TODO: fill mode
	// TODO: cull mode

	// viewport
	if(forceReset || targetState.viewportWidth != boundState.viewportWidth || targetState.viewportHeight != boundState.viewportHeight)
	{
		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = targetState.viewportWidth;
		viewport.Height = targetState.viewportHeight;
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
			blendState = fast_cast<DxBlendState*>(&*targetState.blendState)->GetBlendStateInterface();
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
		deviceContext->DrawIndexed(boundState.indexBuffer->GetIndicesCount(), 0, 0);
	else
		deviceContext->Draw(boundState.vertexBuffer->GetVerticesCount(), 0);
}

void DxContext::DrawInstanced(int instancesCount)
{
	Update();

	if(boundState.indexBuffer)
		deviceContext->DrawIndexedInstanced(boundState.indexBuffer->GetIndicesCount(), instancesCount, 0, 0, 0);
	else
		deviceContext->DrawInstanced(boundState.vertexBuffer->GetVerticesCount(), instancesCount, 0, 0);
}
