#include "Context.hpp"
#include "RenderBuffer.hpp"
#include "DepthStencilBuffer.hpp"
#include "Texture.hpp"
#include "SamplerState.hpp"
#include "UniformBuffer.hpp"
#include "VertexShader.hpp"
#include "PixelShader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Geometry.hpp"
#include "BlendState.hpp"

Context::Context() : forceReset(true) {}

ContextState& Context::GetTargetState()
{
	return targetState;
}

void Context::Reset()
{
	forceReset = true;
}
