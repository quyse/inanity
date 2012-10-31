#include "DxContext.hpp"
#include "DxDevice.hpp"
#include "DxRenderBuffer.hpp"
#include "DxDepthStencilBuffer.hpp"
#include "DxTexture.hpp"
#include "DxSampler.hpp"
#include "DxUniformBuffer.hpp"
#include "DxVertexShader.hpp"
#include "DxPixelShader.hpp"
#include "DxVertexBuffer.hpp"
#include "DxInternalInputLayout.hpp"
#include "DxInternalInputLayoutCache.hpp"
#include "DxIndexBuffer.hpp"
#include "Layout.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

DxContext::DxContext(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
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
			semanticName.assign('A', 8);
			int semantic = element.semantic;
			for(int j = 0; semantic; ++j)
			{
				semanticName[j] = 'A' + semantic % 26;
				semantic /= 26;
			}

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
		// сейчас единственная оптимизация - количество обновляемых рендертаргетов
		int end;
		for(end = ContextState::renderTargetSlotsCount - 1; end >= 0; --end)
			if(targetState.renderBuffers[end] != boundState.renderBuffers[end])
				break;
		++end;

		if(end > 0 || targetState.depthStencilBuffer != boundState.depthStencilBuffer)
		{
			ID3D11RenderTargetView* views[ContextState::renderTargetSlotsCount];
			for(int i = 0; i < end; ++i)
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
			deviceContext->OMSetRenderTargets(end, views, depthStencilView);

			// обновить актуальное состояние
			for(int i = 0; i < end; ++i)
				boundState.renderBuffers[i] = targetState.renderBuffers[i];
			boundState.depthStencilBuffer = targetState.depthStencilBuffer;
		}
	}

	// текстуры
	{
		// сейчас единственная оптимизация - начало и конец в списке обновляемых текстур
		int end;
		for(end = ContextState::textureSlotsCount - 1; end >= 0; --end)
			if(targetState.textures[end] != boundState.textures[end])
				break;
		++end;
		int begin;
		for(begin = 0; begin < end; ++begin)
			if(targetState.textures[begin] != boundState.textures[begin])
				break;

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
		for(end = ContextState::textureSlotsCount - 1; end >= 0; --end)
			if(targetState.samplers[end] != boundState.samplers[end])
				break;
		++end;
		int begin;
		for(begin = 0; begin < end; ++begin)
			if(targetState.samplers[begin] != boundState.samplers[begin])
				break;

		if(begin < end)
		{
			ID3D11SamplerState* states[ContextState::textureSlotsCount];
			for(int i = begin; i < end; ++i)
			{
				Sampler* abstractSampler = targetState.samplers[i];
				if(abstractSampler)
				{
					DxSampler* sampler = fast_cast<DxSampler*>(abstractSampler);
					states[i] = sampler->GetSamplerStateInterface();
				}
				else
					states[i] = 0;
			}

			// установить семплеры
			deviceContext->VSSetSamplers(begin, end - begin, states + begin);
			deviceContext->PSSetSamplers(begin, end - begin, states + begin);

			// обновить актуальное состояние
			for(int i = begin; i < end; ++i)
				boundState.samplers[i] = targetState.samplers[i];
		}
	}

	// константные буферы
	{
		// сейчас единственная оптимизация - начало и конец в списке обновляемых текстур
		int end;
		for(end = ContextState::uniformBufferSlotsCount - 1; end >= 0; --end)
			if(targetState.uniformBuffers[end] != boundState.uniformBuffers[end])
				break;
		++end;
		int begin;
		for(begin = 0; begin < end; ++begin)
			if(targetState.uniformBuffers[begin] != boundState.uniformBuffers[begin])
				break;

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
	if(targetState.vertexBuffer != boundState.vertexBuffer || targetState.vertexShader != boundState.vertexShader)
	{
		ptr<DxInternalInputLayout> inputLayout = inputLayoutCache->GetInputLayout(targetState.vertexBuffer->GetLayout(), fast_cast<DxVertexShader*>(&*targetState.vertexShader));
		if(inputLayout != boundInputLayout)
		{
			deviceContext->IASetInputLayout(inputLayout->GetInputLayoutInterface());
			boundInputLayout = inputLayout;
		}
	}

	// вершинный шейдер
	if(targetState.vertexShader != boundState.vertexShader)
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
	if(targetState.pixelShader != boundState.pixelShader)
	{
		ID3D11PixelShader* shader;
		if(targetState.pixelShader)
			shader = fast_cast<DxPixelShader*>(&*targetState.pixelShader)->GetPixelShaderInterface();
		else
			shader = 0;
		deviceContext->PSSetShader(shader, NULL, 0);

		boundState.pixelShader = targetState.pixelShader;
	}
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
