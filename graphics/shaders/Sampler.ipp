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
Value<ValueType> Sampler<ValueType, dimensions>::SampleOffset(Value<CoordType> coords, Value<IntCoordType> offset)
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
Value<ValueType> Sampler<ValueType, dimensions>::SampleLod(Value<CoordType> coords, Value<float> lod)
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
Value<ValueType> Sampler<ValueType, dimensions>::SampleLodOffset(Value<CoordType> coords, Value<float> lod, Value<IntCoordType> offset)
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
Value<ValueType> Sampler<ValueType, dimensions>::SampleBias(Value<CoordType> coords, Value<float> bias)
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
Value<ValueType> Sampler<ValueType, dimensions>::SampleBiasOffset(Value<CoordType> coords, Value<float> bias, Value<IntCoordType> offset)
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
Value<ValueType> Sampler<ValueType, dimensions>::SampleGrad(Value<CoordType> coords, Value<CoordType> gradX, Value<CoordType> gradY)
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
Value<ValueType> Sampler<ValueType, dimensions>::SampleGradOffset(Value<CoordType> coords, Value<CoordType> gradX, Value<CoordType> gradY, Value<IntCoordType> offset)
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
SamplerCube<ValueType>::SamplerCube(ptr<SamplerNode> node)
: SamplerBase(node) {}

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
