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

	fillMode = fillModeSolid;
	dirtyFillMode = true;
	cullMode = cullModeBack;
	dirtyCullMode = true;

	viewportWidth = viewportHeight = 1;
	dirtyViewport = true;

	depthTestFunc = depthTestFuncLess;
	dirtyDepthTestFunc = true;
	depthWrite = true;
	dirtyDepthWrite = true;
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
		boundTextures[slot] = texture;
		dirtyTextures[slot] = true;
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

void Context::SetFillMode(FillMode fillMode)
{
	if(this->fillMode != fillMode)
	{
		this->fillMode = fillMode;
		dirtyFillMode = true;
	}
}

void Context::SetCullMode(CullMode cullMode)
{
	if(this->cullMode != cullMode)
	{
		this->cullMode = cullMode;
		dirtyCullMode = true;
	}
}

void Context::SetViewport(int viewportWidth, int viewportHeight)
{
	if(this->viewportWidth != viewportWidth || this->viewportHeight != viewportHeight)
	{
		this->viewportWidth = viewportWidth;
		this->viewportHeight = viewportHeight;
		dirtyViewport = true;
	}
}

void Context::SetDepthTestFunc(DepthTestFunc depthTestFunc)
{
	if(this->depthTestFunc != depthTestFunc)
	{
		this->depthTestFunc = depthTestFunc;
		dirtyDepthTestFunc = true;
	}
}

void Context::SetDepthWrite(bool depthWrite)
{
	if(this->depthWrite != depthWrite)
	{
		this->depthWrite = depthWrite;
		dirtyDepthWrite = true;
	}
}
