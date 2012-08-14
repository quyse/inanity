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
#include "VertexLayout.hpp"
#include "DxInternalInputLayout.hpp"
#include "DxInternalInputLayoutCache.hpp"
#include "DxIndexBuffer.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

DxContext::DxContext(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
: device(device), deviceContext(deviceContext)
{
	// создать кэш входных разметок
	inputLayoutCache = NEW(DxInternalInputLayoutCache(this));
}

ptr<DxInternalInputLayout> DxContext::CreateInternalInputLayout(VertexLayout* vertexLayout, DxVertexShader* vertexShader)
{
	try
	{
		// получить элементы разметки
		const std::vector<VertexLayout::Element>& elements = vertexLayout->GetElements();
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
			const VertexLayout::Element& element = elements[i];

			// получить количество необходимых описаний для элемента
			int needDescsCount;
			switch(element.type)
			{
			case VertexLayout::typeFloat4x4:
				needDescsCount = 4;
				break;
			default:
				needDescsCount = 1;
				break;
			}

			// выбрать формат элемента
			DXGI_FORMAT format;
			switch(element.type)
			{
			case VertexLayout::typeFloat:
				format = DXGI_FORMAT_R32_FLOAT;
				break;
			case VertexLayout::typeFloat2:
				format = DXGI_FORMAT_R32G32_FLOAT;
				break;
			case VertexLayout::typeFloat3:
				format = DXGI_FORMAT_R32G32B32_FLOAT;
				break;
			case VertexLayout::typeFloat4:
				format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				break;
			case VertexLayout::typeFloat4x4:
				format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				break;
			case VertexLayout::typeUInt:
				format = DXGI_FORMAT_R32_UINT;
				break;
			case VertexLayout::typeUInt2:
				format = DXGI_FORMAT_R32G32_UINT;
				break;
			case VertexLayout::typeUInt3:
				format = DXGI_FORMAT_R32G32B32_UINT;
				break;
			case VertexLayout::typeUInt4:
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
		for(end = renderTargetSlotsCount - 1; end >= 0; --end)
			if(dirtyRenderBuffers[end])
				break;
		++end;

		if(end > 0 || dirtyDepthStencilBuffer)
		{
			ID3D11RenderTargetView* views[renderTargetSlotsCount];
			for(int i = 0; i < end; ++i)
			{
				RenderBuffer* abstractRenderBuffer = boundRenderBuffers[i];
				if(abstractRenderBuffer)
				{
					DxRenderBuffer* renderBuffer = fast_cast<DxRenderBuffer*>(abstractRenderBuffer);
					views[i] = renderBuffer->GetRenderTargetViewInterface();
				}
				else
					views[i] = 0;
			}

			ID3D11DepthStencilView* depthStencilView;
			DepthStencilBuffer* abstractDepthStencilBuffer = boundDepthStencilBuffer;
			if(abstractDepthStencilBuffer)
			{
				DxDepthStencilBuffer* depthStencilBuffer = fast_cast<DxDepthStencilBuffer*>(abstractDepthStencilBuffer);
				depthStencilView = depthStencilBuffer->GetDepthStencilViewInterface();
			}
			else
				depthStencilView = 0;

			// выполнить вызов
			deviceContext->OMSetRenderTargets(end, views, depthStencilView);

			// обновить флажки
			for(int i = 0; i < end; ++i)
				dirtyRenderBuffers[i] = false;
			dirtyDepthStencilBuffer = false;
		}
	}

	// текстуры
	{
		// сейчас единственная оптимизация - начало и конец в списке обновляемых текстур
		int end;
		for(end = textureSlotsCount - 1; end >= 0; --end)
			if(dirtyTextures[end])
				break;
		++end;
		int begin;
		for(begin = 0; begin < end; ++begin)
			if(dirtyTextures[begin])
				break;

		if(begin < end)
		{
			ID3D11ShaderResourceView* views[textureSlotsCount];
			for(int i = begin; i < end; ++i)
			{
				Texture* abstractTexture = boundTextures[i];
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

			// обновить флажки
			for(int i = begin; i < end; ++i)
				dirtyTextures[i] = false;
		}
	}

	// семплеры
	{
		// сейчас единственная оптимизация - начало и конец в списке обновляемых семплеров
		int end;
		for(end = textureSlotsCount - 1; end >= 0; --end)
			if(dirtySamplers[end])
				break;
		++end;
		int begin;
		for(begin = 0; begin < end; ++begin)
			if(dirtySamplers[begin])
				break;

		if(begin < end)
		{
			ID3D11SamplerState* states[textureSlotsCount];
			for(int i = begin; i < end; ++i)
			{
				Sampler* abstractSampler = boundSamplers[i];
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

			// обновить флажки
			for(int i = begin; i < end; ++i)
				dirtySamplers[i] = false;
		}
	}

	// константные буферы
	{
		// сейчас единственная оптимизация - начало и конец в списке обновляемых текстур
		int end;
		for(end = uniformBufferSlotsCount - 1; end >= 0; --end)
			if(dirtyUniformBuffers[end])
				break;
		++end;
		int begin;
		for(begin = 0; begin < end; ++begin)
			if(dirtyUniformBuffers[begin])
				break;

		if(begin < end)
		{
			ID3D11Buffer* buffers[uniformBufferSlotsCount];
			for(int i = begin; i < end; ++i)
			{
				UniformBuffer* abstractUniformBuffer = boundUniformBuffers[i];
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

			// обновить флажки
			for(int i = begin; i < end; ++i)
				dirtyUniformBuffers[i] = false;
		}
	}

	// входная разметка
	if(dirtyVertexBuffer || dirtyVertexShader)
	{
		ptr<DxInternalInputLayout> inputLayout = inputLayoutCache->GetInputLayout(boundVertexBuffer->GetVertexLayout(), fast_cast<DxVertexShader*>(&*boundVertexShader));
		if(inputLayout != boundInputLayout)
		{
			deviceContext->IASetInputLayout(inputLayout->GetInputLayoutInterface());
			boundInputLayout = inputLayout;
		}
	}

	// вершинный шейдер
	if(dirtyVertexShader)
	{
		ID3D11VertexShader* shader;
		if(boundVertexShader)
			shader = fast_cast<DxVertexShader*>(&*boundVertexShader)->GetVertexShaderInterface();
		else
			shader = 0;
		deviceContext->VSSetShader(shader, NULL, 0);

		dirtyVertexShader = false;
	}

	// пиксельный шейдер
	if(dirtyPixelShader)
	{
		ID3D11PixelShader* shader;
		if(boundPixelShader)
			shader = fast_cast<DxPixelShader*>(&*boundPixelShader)->GetPixelShaderInterface();
		else
			shader = 0;
		deviceContext->PSSetShader(shader, NULL, 0);

		dirtyPixelShader = false;
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

	if(boundIndexBuffer)
		deviceContext->DrawIndexed(boundIndexBuffer->GetIndicesCount(), 0, 0);
	else
		deviceContext->Draw(boundVertexBuffer->GetVerticesCount(), 0);
}

void DxContext::DrawInstanced(int instancesCount)
{
	Update();

	if(boundIndexBuffer)
		deviceContext->DrawIndexedInstanced(boundIndexBuffer->GetIndicesCount(), instancesCount, 0, 0, 0);
	else
		deviceContext->DrawInstanced(boundVertexBuffer->GetVerticesCount(), instancesCount, 0, 0);
}
