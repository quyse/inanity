#include "Sampler.hpp"
#include "SamplerNode.hpp"
#include "../Texture.hpp"
#include "../SamplerState.hpp"
#include "../ContextState.hpp"

BEGIN_INANITY_SHADERS

SamplerBase::SamplerBase(ptr<SamplerNode> node)
: Expression(node) {}

void SamplerBase::SetTexture(ptr<Texture> texture)
{
	this->texture = texture;
}

void SamplerBase::SetSamplerState(ptr<SamplerState> samplerState)
{
	this->samplerState = samplerState;
}

void SamplerBase::Apply(ContextState& contextState) const
{
	node.FastCast<SamplerNode>()->Apply(contextState, texture, samplerState);
}

END_INANITY_SHADERS
