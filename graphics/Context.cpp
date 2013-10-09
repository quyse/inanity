#include "Context.hpp"
#include "FrameBuffer.hpp"
#include "RenderBuffer.hpp"
#include "DepthStencilBuffer.hpp"
#include "Texture.hpp"
#include "SamplerState.hpp"
#include "UniformBuffer.hpp"
#include "VertexShader.hpp"
#include "PixelShader.hpp"
#include "AttributeBinding.hpp"
#include "AttributeLayoutSlot.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "BlendState.hpp"
#include "shaders/Sampler.hpp"
#include "shaders/UniformGroup.hpp"

BEGIN_INANITY_GRAPHICS

//*** Context::Let

Context::Let::Let()
: cell(0), next(0) {}

void Context::Let::Init(Cell* cell)
{
	THROW_ASSERT(!this->cell);
	this->cell = cell;

	next = cell->top;
	cell->top = this;
}

Context::Let::~Let()
{
	if(cell)
	{
		if(cell->current == this)
			cell->current = 0;
		THROW_ASSERT(cell->top == this);
		cell->top = next;
	}
}

//*** Context::Cell

Context::Cell::Cell() : top(0), current(0) {}

bool Context::Cell::IsActual()
{
	return current == top;
}

void Context::Cell::Actual()
{
	current = top;
}

void Context::Cell::Reset()
{
	current = 0;
}

//*** Context::LetFrameBuffer

Context::LetFrameBuffer::LetFrameBuffer() {}

Context::LetFrameBuffer::LetFrameBuffer(Context* context, ptr<FrameBuffer> frameBuffer)
{
	operator()(context, frameBuffer);
}

void Context::LetFrameBuffer::operator()(Context* context, ptr<FrameBuffer> frameBuffer)
{
	Init(&context->cellFrameBuffer);
	this->frameBuffer = frameBuffer;
}

//*** Context::LetSampler

Context::LetSampler::LetSampler() {}

Context::LetSampler::LetSampler(Context* context, int i, ptr<Texture> texture, ptr<SamplerState> samplerState)
{
	operator()(context, i, texture, samplerState);
}

Context::LetSampler::LetSampler(Context* context, const Shaders::SamplerBase& sampler, ptr<Texture> texture, ptr<SamplerState> samplerState)
{
	operator()(context, sampler, texture, samplerState);
}

void Context::LetSampler::operator()(Context* context, int i, ptr<Texture> texture, ptr<SamplerState> samplerState)
{
	Init(&context->cellSamplers[i]);
	this->texture = texture;
	this->samplerState = samplerState;
}

void Context::LetSampler::operator()(Context* context, const Shaders::SamplerBase& sampler, ptr<Texture> texture, ptr<SamplerState> samplerState)
{
	Init(&context->cellSamplers[sampler.GetSlot()]);
	this->texture = texture;
	this->samplerState = samplerState;
}

//*** Context::LetUniformBuffer

Context::LetUniformBuffer::LetUniformBuffer() {}

Context::LetUniformBuffer::LetUniformBuffer(Context* context, int i, ptr<UniformBuffer> uniformBuffer)
{
	operator()(context, i, uniformBuffer);
}

Context::LetUniformBuffer::LetUniformBuffer(Context* context, Shaders::UniformGroup* uniformGroup)
{
	operator()(context, uniformGroup);
}

void Context::LetUniformBuffer::operator()(Context* context, int i, ptr<UniformBuffer> uniformBuffer)
{
	Init(&context->cellUniformBuffers[i]);
	this->uniformBuffer = uniformBuffer;
}

void Context::LetUniformBuffer::operator()(Context* context, Shaders::UniformGroup* uniformGroup)
{
	Init(&context->cellUniformBuffers[uniformGroup->GetSlot()]);
	this->uniformBuffer = uniformGroup->GetBuffer();
}

//*** Context::LetVertexShader

Context::LetVertexShader::LetVertexShader() {}

Context::LetVertexShader::LetVertexShader(Context* context, ptr<VertexShader> vertexShader)
{
	operator()(context, vertexShader);
}

void Context::LetVertexShader::operator()(Context* context, ptr<VertexShader> vertexShader)
{
	Init(&context->cellVertexShader);
	this->vertexShader = vertexShader;
}

//*** Context::LetPixelShader

Context::LetPixelShader::LetPixelShader() {}

Context::LetPixelShader::LetPixelShader(Context* context, ptr<PixelShader> pixelShader)
{
	operator()(context, pixelShader);
}

void Context::LetPixelShader::operator()(Context* context, ptr<PixelShader> pixelShader)
{
	Init(&context->cellPixelShader);
	this->pixelShader = pixelShader;
}

//*** Context::LetAttributeBinding

Context::LetAttributeBinding::LetAttributeBinding() {}

Context::LetAttributeBinding::LetAttributeBinding(Context* context, ptr<AttributeBinding> attributeBinding)
{
	operator()(context, attributeBinding);
}

void Context::LetAttributeBinding::operator()(Context* context, ptr<AttributeBinding> attributeBinding)
{
	Init(&context->cellAttributeBinding);
	this->attributeBinding = attributeBinding;
}

//*** Context::LetVertexBuffer

Context::LetVertexBuffer::LetVertexBuffer() {}

Context::LetVertexBuffer::LetVertexBuffer(Context* context, int i, ptr<VertexBuffer> vertexBuffer)
{
	operator()(context, i, vertexBuffer);
}

Context::LetVertexBuffer::LetVertexBuffer(Context* context, AttributeLayoutSlot* slot, ptr<VertexBuffer> vertexBuffer)
{
	operator()(context, slot, vertexBuffer);
}

void Context::LetVertexBuffer::operator()(Context* context, int i, ptr<VertexBuffer> vertexBuffer)
{
	Init(&context->cellVertexBuffers[i]);
	this->vertexBuffer = vertexBuffer;
}

void Context::LetVertexBuffer::operator()(Context* context, AttributeLayoutSlot* slot, ptr<VertexBuffer> vertexBuffer)
{
	Init(&context->cellVertexBuffers[slot->GetIndex()]);
	this->vertexBuffer = vertexBuffer;
}

//*** Context::LetIndexBuffer

Context::LetIndexBuffer::LetIndexBuffer() {}

Context::LetIndexBuffer::LetIndexBuffer(Context* context, ptr<IndexBuffer> indexBuffer)
{
	operator()(context, indexBuffer);
}

void Context::LetIndexBuffer::operator()(Context* context, ptr<IndexBuffer> indexBuffer)
{
	Init(&context->cellIndexBuffer);
	this->indexBuffer = indexBuffer;
}

//*** Context::LetFillMode

Context::LetFillMode::LetFillMode() {}

Context::LetFillMode::LetFillMode(Context* context, FillMode fillMode)
{
	operator()(context, fillMode);
}

void Context::LetFillMode::operator()(Context* context, FillMode fillMode)
{
	Init(&context->cellFillMode);
	this->fillMode = fillMode;
}

//*** Context::LetCullMode

Context::LetCullMode::LetCullMode() {}

Context::LetCullMode::LetCullMode(Context* context, CullMode cullMode)
{
	operator()(context, cullMode);
}

void Context::LetCullMode::operator()(Context* context, CullMode cullMode)
{
	Init(&context->cellCullMode);
	this->cullMode = cullMode;
}

//*** Context::LetViewport

Context::LetViewport::LetViewport() {}

Context::LetViewport::LetViewport(Context* context, int viewportWidth, int viewportHeight)
{
	operator()(context, viewportWidth, viewportHeight);
}

void Context::LetViewport::operator()(Context* context, int viewportWidth, int viewportHeight)
{
	Init(&context->cellViewport);
	this->viewportWidth = viewportWidth;
	this->viewportHeight = viewportHeight;
}

//*** Context::LetDepthTestFunc

Context::LetDepthTestFunc::LetDepthTestFunc() {}

Context::LetDepthTestFunc::LetDepthTestFunc(Context* context, DepthTestFunc depthTestFunc)
{
	operator()(context, depthTestFunc);
}

void Context::LetDepthTestFunc::operator()(Context* context, DepthTestFunc depthTestFunc)
{
	Init(&context->cellDepthTestFunc);
	this->depthTestFunc = depthTestFunc;
}

//*** Context::LetDepthWrite

Context::LetDepthWrite::LetDepthWrite() {}

Context::LetDepthWrite::LetDepthWrite(Context* context, bool depthWrite)
{
	operator()(context, depthWrite);
}

void Context::LetDepthWrite::operator()(Context* context, bool depthWrite)
{
	Init(&context->cellDepthWrite);
	this->depthWrite = depthWrite;
}

//*** Context::LetBlendState

Context::LetBlendState::LetBlendState() {}

Context::LetBlendState::LetBlendState(Context* context, ptr<BlendState> blendState)
{
	operator()(context, blendState);
}

void Context::LetBlendState::operator()(Context* context, ptr<BlendState> blendState)
{
	Init(&context->cellBlendState);
	this->blendState = blendState;
}

//*** Context

Context::Context() {}

END_INANITY_GRAPHICS
