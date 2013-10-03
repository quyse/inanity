#include "Sampler.hpp"
#include "SamplerNode.hpp"

BEGIN_INANITY_SHADERS

SamplerBase::SamplerBase(ptr<SamplerNode> node)
: Expression(node) {}

int SamplerBase::GetSlot() const
{
	return fast_cast<SamplerNode*>(&*node)->GetSlot();
}

END_INANITY_SHADERS
