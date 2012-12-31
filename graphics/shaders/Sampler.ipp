#ifndef ___INANITY_GRAPHICS_SHADERS_SAMPLER_IPP___
#define ___INANITY_GRAPHICS_SHADERS_SAMPLER_IPP___

#include "Sampler.hpp"
#include "SamplerNode.hpp"
#include "SampleNode.hpp"

BEGIN_INANITY_SHADERS

template <typename ValueType, typename CoordType>
Sampler<ValueType, CoordType>::Sampler(ptr<SamplerNode> node)
: Expression(node) {}

template <typename ValueType, typename CoordType>
Sampler<ValueType, CoordType>::Sampler(int slot)
: Expression(NEW(SamplerNode(slot, GetDataType<ValueType>(), GetDataType<CoordType>()))) {}

template <typename ValueType, typename CoordType>
void Sampler<ValueType, CoordType>::operator=(Sampler<ValueType, CoordType> a)
{
	// реализации не требуется
}

template <typename ValueType, typename CoordType>
Value<ValueType> Sampler<ValueType, CoordType>::Sample(Value<CoordType> coords)
{
	return NEW(SampleNode(node.FastCast<SamplerNode>(), coords.GetNode()));
}

END_INANITY_SHADERS

#endif
