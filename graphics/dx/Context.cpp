#include "Context.hpp"
#include "RenderBuffer.hpp"
#include "DepthStencilBuffer.hpp"
#include "ConstantBuffer.hpp"
#include "BlendState.hpp"
#include "DepthStencilState.hpp"
#include "RasterizerState.hpp"
#include "SamplerState.hpp"
#include "Texture.hpp"
#include "Geometry.hpp"
#include "ShaderLayout.hpp"
#include "VertexShader.hpp"
#include "PixelShader.hpp"

Context::Context(ID3D11DeviceContext* context)
	: context(context), lastViewportWidth(0), lastViewportHeight(0)
{
}

ID3D11DeviceContext* Context::GetDeviceContext() const
{
	return context;
}

void Context::SetViewport(size_t width, size_t height)
{
	if(width == lastViewportWidth && height == lastViewportHeight) return;

	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	context->RSSetViewports(1, &viewport);

	lastViewportWidth = width;
	lastViewportHeight = height;
}

void Context::ClearRenderBuffer(RenderBuffer* renderBuffer, const float* color)
{
	context->ClearRenderTargetView(renderBuffer->GetRenderTargetView(), color);
}

void Context::ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, float depth)
{
	context->ClearDepthStencilView(depthStencilBuffer->GetDepthStencilView(), D3D11_CLEAR_DEPTH, depth, 0);
}

void Context::ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, unsigned stencil)
{
	context->ClearDepthStencilView(depthStencilBuffer->GetDepthStencilView(), D3D11_CLEAR_STENCIL, 0, stencil);
}

void Context::ClearDepthStencilBuffer(DepthStencilBuffer* depthStencilBuffer, float depth, unsigned stencil)
{
	context->ClearDepthStencilView(depthStencilBuffer->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}

void Context::SetRenderTargets(const std::vector<ptr<RenderBuffer> >& renderBuffers, ptr<DepthStencilBuffer> depthStencilBuffer)
{
	renderTargets.resize(renderBuffers.size());
	for(size_t i = 0; i < renderBuffers.size(); ++i)
		renderTargets[i] = renderBuffers[i] ? renderBuffers[i]->GetRenderTargetView() : 0;
	context->OMSetRenderTargets(renderTargets.size(), renderTargets.empty() ? 0 : &renderTargets[0], depthStencilBuffer ? depthStencilBuffer->GetDepthStencilView() : 0);
	renderTargets.clear();
}

void Context::SetBufferData(ConstantBuffer* buffer, const void* data, size_t size)
{
	//спроецировать буфер
	D3D11_MAPPED_SUBRESOURCE bufferData;
	ID3D11Resource* resource = buffer->GetInternalInterface();
	if(FAILED(context->Map(resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData)))
		THROW_PRIMARY_EXCEPTION("Can't map graphics buffer");

	//скопировать данные
	memcpy(bufferData.pData, data, size);

	//отключить проекцию буфера
	context->Unmap(resource, 0);
}

void Context::SetBlendState(BlendState* blendState)
{
	FLOAT blendFactor[] = { 0, 0, 0, 0 };
	context->OMSetBlendState(blendState->GetInternalInterface(), blendFactor, 0xFFFFFFFF);
}

void Context::SetDepthStencilState(DepthStencilState* depthStencilState)
{
	context->OMSetDepthStencilState(depthStencilState->GetInternalInterface(), 0);
}

void Context::SetRasterizerState(RasterizerState* rasterizerState)
{
	context->RSSetState(rasterizerState->GetInternalInterface());
}

void Context::SetSamplerState(unsigned slot, unsigned stages, SamplerState* samplerState)
{
	ID3D11SamplerState* samplerStateInterface = samplerState->GetInternalInterface();

	if(stages & stageVertex)
		context->VSSetSamplers(slot, 1, &samplerStateInterface);
	if(stages & stagePixel)
		context->PSSetSamplers(slot, 1, &samplerStateInterface);
}

void Context::SetSamplerStates(unsigned startSlot, unsigned stages, SamplerState** samplerStates, unsigned count)
{
	ID3D11SamplerState** samplerStateInterfaces = (ID3D11SamplerState**)alloca(count * sizeof(ID3D11SamplerState*));
	for(unsigned i = 0; i < count; ++i)
		samplerStateInterfaces[i] = samplerStates[i] ? samplerStates[i]->GetInternalInterface() : nullptr;
	while(count && !samplerStateInterfaces[count - 1])
		--count;

	if(stages & stageVertex)
		context->VSSetSamplers(startSlot, count, samplerStateInterfaces);
	if(stages & stagePixel)
		context->PSSetSamplers(startSlot, count, samplerStateInterfaces);
}

void Context::SetTexture(unsigned slot, unsigned stages, Texture* texture)
{
	ID3D11ShaderResourceView* resource = texture->GetShaderResource();

	if(stages & stageVertex)
		context->VSSetShaderResources(slot, 1, &resource);
	if(stages & stagePixel)
		context->PSSetShaderResources(slot, 1, &resource);
}

void Context::SetTextures(unsigned startSlot, unsigned stages, Texture* texture1, Texture* texture2, Texture* texture3, Texture* texture4)
{
	ID3D11ShaderResourceView* textures[4];
	textures[0] = texture1 ? texture1->GetShaderResource() : nullptr;
	textures[1] = texture2 ? texture2->GetShaderResource() : nullptr;
	textures[2] = texture3 ? texture3->GetShaderResource() : nullptr;
	textures[3] = texture4 ? texture4->GetShaderResource() : nullptr;
	unsigned count;
	for(count = 0; count < 4 && textures[count]; ++count);

	if(stages & stageVertex)
		context->VSSetShaderResources(startSlot, count, textures);
	if(stages & stagePixel)
		context->PSSetShaderResources(startSlot, count, textures);
}

void Context::SetTextures(unsigned startSlot, unsigned stages, Texture** textures, unsigned count)
{
	ID3D11ShaderResourceView** views = (ID3D11ShaderResourceView**)alloca(count * sizeof(ID3D11ShaderResourceView*));
	for(unsigned i = 0; i < count; ++i)
		views[i] = textures[i] ? textures[i]->GetShaderResource() : nullptr;
	while(count && !views[count - 1])
		--count;

	if(stages & stageVertex)
		context->VSSetShaderResources(startSlot, count, views);
	if(stages & stagePixel)
		context->PSSetShaderResources(startSlot, count, views);
}

void Context::SetConstantBuffer(unsigned slot, unsigned stages, ConstantBuffer* buffer)
{
	ID3D11Buffer* bufferInterface = buffer->GetInternalInterface();

	if(stages & stageVertex)
		context->VSSetConstantBuffers(slot, 1, &bufferInterface);
	if(stages & stagePixel)
		context->PSSetConstantBuffers(slot, 1, &bufferInterface);
}

void Context::SetConstantBuffers(unsigned startSlot, unsigned stages, ConstantBuffer* buffer1, ConstantBuffer* buffer2, ConstantBuffer* buffer3, ConstantBuffer* buffer4)
{
	ID3D11Buffer* buffers[4];
	buffers[0] = buffer1 ? buffer1->GetInternalInterface() : nullptr;
	buffers[1] = buffer2 ? buffer2->GetInternalInterface() : nullptr;
	buffers[2] = buffer3 ? buffer3->GetInternalInterface() : nullptr;
	buffers[3] = buffer4 ? buffer4->GetInternalInterface() : nullptr;
	unsigned count;
	for(count = 0; count < 4 && buffers[count]; ++count);

	if(stages & stageVertex)
		context->VSSetConstantBuffers(startSlot, count, buffers);
	if(stages & stagePixel)
		context->PSSetConstantBuffers(startSlot, count, buffers);
}

void Context::SetConstantBuffers(unsigned startSlot, unsigned stages, ConstantBuffer** buffers, unsigned count)
{
	ID3D11Buffer** intBuffers = (ID3D11Buffer**)alloca(count * sizeof(ID3D11Buffer*));
	for(unsigned i = 0; i < count; ++i)
		intBuffers[i] = buffers[i] ? buffers[i]->GetInternalInterface() : nullptr;
	while(count && !intBuffers[count - 1])
		--count;

	if(stages & stageVertex)
		context->VSSetConstantBuffers(startSlot, count, intBuffers);
	if(stages & stagePixel)
		context->PSSetConstantBuffers(startSlot, count, intBuffers);
}

void Context::ApplyGeometry(Geometry* geometry)
{
	ID3D11Buffer* vertexBuffer = geometry->GetVertexBuffer()->GetInternalInterface();
	UINT stride = geometry->GetVertexStride();
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	Buffer* indexBuffer = geometry->GetIndexBuffer();
	if(indexBuffer)
		context->IASetIndexBuffer(geometry->GetIndexBuffer()->GetInternalInterface(), geometry->GetIndexFormat(), 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Context::ApplyShaderLayout(ShaderLayout* shaderLayout)
{
	context->IASetInputLayout(shaderLayout->GetInternalInterface());
}

void Context::ApplyShaders(VertexShader* vertexShader, PixelShader* pixelShader)
{
	context->VSSetShader(vertexShader ? vertexShader->GetInternalInterface() : nullptr, NULL, 0);
	context->PSSetShader(pixelShader ? pixelShader->GetInternalInterface() : nullptr, NULL, 0);
}

void Context::DrawGeometry(unsigned indicesCount, bool indexed, unsigned instancesCount)
{
	if(instancesCount <= 1)
		if(indexed)
			context->DrawIndexed(indicesCount, 0, 0);
		else
			context->Draw(indicesCount, 0);
	else
		if(indexed)
			context->DrawIndexedInstanced(indicesCount, instancesCount, 0, 0, 0);
		else
			context->DrawInstanced(indicesCount, instancesCount, 0, 0);
}
