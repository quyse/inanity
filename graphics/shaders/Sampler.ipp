#ifndef ___INANITY_GRAPHICS_SHADERS_SAMPLER_IPP___
#define ___INANITY_GRAPHICS_SHADERS_SAMPLER_IPP___

#include "Sampler.hpp"
#include "SamplerNode.hpp"
#include "SampleNode.hpp"

BEGIN_INANITY_SHADERS

//*** class Sampler

template <typename ValueType, int dimensions>
Sampler<ValueType, dimensions>::Sampler(ptr<SamplerNode> node)
: SamplerBase(node) {}

template <typename ValueType, int dimensions>
Sampler<ValueType, dimensions>::Sampler(int slot)
: SamplerBase(NEW(SamplerNode(slot, DataTypeOf<ValueType>(), SamplerNode::CoordTypeFromDimensions(dimensions)))) {}

template <typename ValueType, int dimensions>
Value<ValueType> Sampler<ValueType, dimensions>::Sample(Value<CoordType> coords)
{
	return Value<ValueType>(NEW(SampleNode(node.FastCast<SamplerNode>(), coords.GetNode())));
}

//*** class SamplerCube

template <typename ValueType>
SamplerCube<ValueType>::SamplerCube(ptr<SamplerNode> node)
: SamplerBase(node) {}

template <typename ValueType>
SamplerCube<ValueType>::SamplerCube(int slot)
: SamplerBase(NEW(SamplerNode(slot, DataTypeOf<ValueType>(), SamplerNode::_Cube))) {}

template <typename ValueType>
Value<ValueType> SamplerCube<ValueType>::Sample(Value<vec3> coords)
{
	return Value<ValueType>(NEW(SampleNode(node.FastCast<SamplerNode>(), coords.GetNode())));
}

END_INANITY_SHADERS

#endif
