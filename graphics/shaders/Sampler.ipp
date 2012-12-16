#ifndef ___INANITY_GRAPHICS_SHADERS_SAMPLER_IPP___
#define ___INANITY_GRAPHICS_SHADERS_SAMPLER_IPP___

#include "Sampler.hpp"
#include "SampleNode.hpp"

BEGIN_INANITY_SHADERS

template <typename ValueType, typename CoordType>
Sampler<ValueType, CoordType>::Sampler(ptr<SamplerNode> node)
: node(node) {}

template <typename ValueType, typename CoordType>
void Sampler<ValueType, CoordType>::operator=(Sampler<ValueType, CoordType> a)
{
	// реализации не требуется
}

template <typename ValueType, typename CoordType>
ptr<SamplerNode> Sampler<ValueType, CoordType>::GetNode() const
{
	return node;
}

template <typename ValueType, typename CoordType>
ValueExpression<ValueType> Sampler<ValueType, CoordType>::Sample(ValueExpression<CoordType> coords)
{
	return NEW(SampleNode(node, coords));
}

END_INANITY_SHADERS

#endif
