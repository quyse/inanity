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
Value<ValueType> Sampler<ValueType, dimensions>::Sample(const Value<CoordType>& coords)
{
	return Value<ValueType>(NEW(SampleNode(
		node.FastCast<SamplerNode>(),
		coords.GetNode(),
		nullptr, // offset
		nullptr, // lod
		nullptr, // bias
		nullptr, // gradX
		nullptr // gradY
	)));
}

template <typename ValueType, int dimensions>
Value<ValueType> Sampler<ValueType, dimensions>::SampleOffset(const Value<CoordType>& coords, const Value<IntCoordType>& offset)
{
	return Value<ValueType>(NEW(SampleNode(
		node.FastCast<SamplerNode>(),
		coords.GetNode(),
		offset.GetNode(),
		nullptr, // lod
		nullptr, // bias
		nullptr, // gradX
		nullptr // gradY
	)));
}

template <typename ValueType, int dimensions>
Value<ValueType> Sampler<ValueType, dimensions>::SampleLod(const Value<CoordType>& coords, const Value<float>& lod)
{
	return Value<ValueType>(NEW(SampleNode(
		node.FastCast<SamplerNode>(),
		coords.GetNode(),
		nullptr, // offset
		lod.GetNode(),
		nullptr, // bias
		nullptr, // gradX
		nullptr // gradY
	)));
}

template <typename ValueType, int dimensions>
Value<ValueType> Sampler<ValueType, dimensions>::SampleLodOffset(const Value<CoordType>& coords, const Value<float>& lod, const Value<IntCoordType>& offset)
{
	return Value<ValueType>(NEW(SampleNode(
		node.FastCast<SamplerNode>(),
		coords.GetNode(),
		offset.GetNode(),
		lod.GetNode(),
		nullptr, // bias
		nullptr, // gradX
		nullptr // gradY
	)));
}

template <typename ValueType, int dimensions>
Value<ValueType> Sampler<ValueType, dimensions>::SampleBias(const Value<CoordType>& coords, const Value<float>& bias)
{
	return Value<ValueType>(NEW(SampleNode(
		node.FastCast<SamplerNode>(),
		coords.GetNode(),
		nullptr, // offset
		nullptr, // lod
		bias.GetNode(),
		nullptr, // gradX
		nullptr // gradY
	)));
}

template <typename ValueType, int dimensions>
Value<ValueType> Sampler<ValueType, dimensions>::SampleBiasOffset(const Value<CoordType>& coords, const Value<float>& bias, const Value<IntCoordType>& offset)
{
	return Value<ValueType>(NEW(SampleNode(
		node.FastCast<SamplerNode>(),
		coords.GetNode(),
		offset.GetNode(),
		nullptr, // lod
		bias.GetNode(),
		nullptr, // gradX
		nullptr // gradY
	)));
}

template <typename ValueType, int dimensions>
Value<ValueType> Sampler<ValueType, dimensions>::SampleGrad(const Value<CoordType>& coords, const Value<CoordType>& gradX, const Value<CoordType>& gradY)
{
	return Value<ValueType>(NEW(SampleNode(
		node.FastCast<SamplerNode>(),
		coords.GetNode(),
		nullptr, // offset
		nullptr, // lod
		nullptr, // bias
		gradX.GetNode(),
		gradY.GetNode()
	)));
}

template <typename ValueType, int dimensions>
Value<ValueType> Sampler<ValueType, dimensions>::SampleGradOffset(const Value<CoordType>& coords, const Value<CoordType>& gradX, const Value<CoordType>& gradY, const Value<IntCoordType>& offset)
{
	return Value<ValueType>(NEW(SampleNode(
		node.FastCast<SamplerNode>(),
		coords.GetNode(),
		offset.GetNode(),
		nullptr, // lod
		nullptr, // bias
		gradX.GetNode(),
		gradY.GetNode()
	)));
}


//*** class SamplerCube

template <typename ValueType>
SamplerCube<ValueType>::SamplerCube(int slot)
: SamplerBase(NEW(SamplerNode(slot, DataTypeOf<ValueType>(), SamplerNode::_Cube))) {}

template <typename ValueType>
Value<ValueType> SamplerCube<ValueType>::Sample(Value<vec3> coords)
{
	return Value<ValueType>(NEW(SampleNode(
		node.FastCast<SamplerNode>(),
		coords.GetNode(),
		nullptr, // offset
		nullptr, // lod
		nullptr, // bias
		nullptr, // gradX
		nullptr // gradY
	)));
}

END_INANITY_SHADERS

#endif
