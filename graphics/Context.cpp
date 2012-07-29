#include "Context.hpp"
#include "RenderBuffer.hpp"
#include "DepthStencilBuffer.hpp"
#include "Texture.hpp"
#include "Sampler.hpp"
#include "UniformBuffer.hpp"
#include "VertexShader.hpp"
#include "PixelShader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

Context::Context()
{
	// сначала всё "грязное", потому что неизвестно, что установлено в API
	for(int i = 0; i < renderTargetSlotsCount; ++i)
		dirtyRenderBuffers[i] = true;
	dirtyDepthStencilBuffer = true;
	for(int i = 0; i < textureSlotsCount; ++i)
		dirtyTextures[i] = true;
	for(int i = 0; i < textureSlotsCount; ++i)
		dirtySamplers[i] = true;
	for(int i = 0; i < uniformBufferSlotsCount; ++i)
		dirtyUniformBuffers[i] = true;
	dirtyVertexShader = true;
	dirtyPixelShader = true;
	dirtyVertexBuffer = true;
	dirtyIndexBuffer = true;
}

void Context::ResetTargets()
{
	for(int i = 0; i < renderTargetSlotsCount; ++i)
		if(boundRenderBuffers[i])
		{
			boundRenderBuffers[i] = 0;
			dirtyRenderBuffers[i] = true;
		}
	if(boundDepthStencilBuffer)
	{
		boundDepthStencilBuffer = 0;
		dirtyDepthStencilBuffer = true;
	}
}

void Context::BindTargetRenderBuffer(int slot, ptr<RenderBuffer> renderBuffer)
{
	if(boundRenderBuffers[slot] != renderBuffer)
	{
		boundRenderBuffers[slot] = renderBuffer;
		dirtyRenderBuffers[slot] = true;
	}
}

void Context::BindTargetDepthStencilBuffer(ptr<DepthStencilBuffer> depthStencilBuffer)
{
	if(boundDepthStencilBuffer != depthStencilBuffer)
	{
		boundDepthStencilBuffer = depthStencilBuffer;
		dirtyDepthStencilBuffer = true;
	}
}

void Context::ResetTextures()
{
	for(int i = 0; i < textureSlotsCount; ++i)
		if(boundTextures[i])
		{
			boundTextures[i] = 0;
			dirtyTextures[i] = true;
		}
}

void Context::BindTexture(int slot, ptr<Texture> texture)
{
	if(boundTextures[slot] != texture)
	{
		boundTextures[i] = texture;
		dirtyTextures[i] = true;
	}
}

void Context::ResetSamplers()
{
	for(int i = 0; i < textureSlotsCount; ++i)
		if(boundSamplers[i])
		{
			boundSamplers[i] = 0;
			dirtySamplers[i] = true;
		}
}

void Context::BindSampler(int slot, ptr<Sampler> sampler)
{
	if(boundSamplers[slot] != sampler)
	{
		boundSamplers[slot] = sampler;
		dirtySamplers[slot] = true;
	}
}

void Context::ResetUniformBuffers()
{
	for(int i = 0; i < uniformBufferSlotsCount; ++i)
		if(boundUniformBuffers[i])
		{
			boundUniformBuffers[i] = 0;
			dirtyUniformBuffers[i] = true;
		}
}

void Context::BindUniformBuffer(int slot, ptr<UniformBuffer> uniformBuffer)
{
	if(boundUniformBuffers[slot] != uniformBuffer)
	{
		boundUniformBuffers[slot] = uniformBuffer;
		dirtyUniformBuffers[slot] = true;
	}
}

void Context::BindVertexShader(ptr<VertexShader> vertexShader)
{
	if(boundVertexShader != vertexShader)
	{
		boundVertexShader = vertexShader;
		dirtyVertexShader = true;
	}
}

void Context::BindPixelShader(ptr<PixelShader> pixelShader)
{
	if(boundPixelShader != pixelShader)
	{
		boundPixelShader = pixelShader;
		dirtyPixelShader = true;
	}
}

void Context::BindVertexBuffer(ptr<VertexBuffer> vertexBuffer)
{
	if(boundVertexBuffer != vertexBuffer)
	{
		boundVertexBuffer = vertexBuffer;
		dirtyVertexBuffer = true;
	}
}

void Context::BindIndexBuffer(ptr<IndexBuffer> indexBuffer)
{
	if(boundIndexBuffer != indexBuffer)
	{
		boundIndexBuffer = indexBuffer;
		dirtyIndexBuffer = true;
	}
}
