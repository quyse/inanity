#ifndef ___INANITY_GRAPHICS_SHADERS_SAMPLER_HPP___
#define ___INANITY_GRAPHICS_SHADERS_SAMPLER_HPP___

#include "Expression.hpp"
#include "Value.hpp"

BEGIN_INANITY_SHADERS

class SamplerNode;

class SamplerBase : public Expression
{
protected:
	SamplerBase(ptr<SamplerNode> node);

public:
	int GetSlot() const;
};

/// Sampler expression (1D, 2D or 3D).
template <typename ValueType, int dimensions>
class Sampler : public SamplerBase
{
public:
	typedef typename generalxvec<float, dimensions>::Type CoordType;
	typedef typename generalxvec<int, dimensions>::Type IntCoordType;

public:
	Sampler(ptr<SamplerNode> node);
	Sampler(int slot);

	Value<ValueType> Sample(const Value<CoordType>& coords);
	Value<ValueType> SampleOffset(const Value<CoordType>& coords, const Value<IntCoordType>& offset);
	Value<ValueType> SampleLod(const Value<CoordType>& coords, const Value<float>& lod);
	Value<ValueType> SampleLodOffset(const Value<CoordType>& coords, const Value<float>& lod, const Value<IntCoordType>& offset);
	Value<ValueType> SampleBias(const Value<CoordType>& coords, const Value<float>& bias);
	Value<ValueType> SampleBiasOffset(const Value<CoordType>& coords, const Value<float>& bias, const Value<IntCoordType>& offset);
	Value<ValueType> SampleGrad(const Value<CoordType>& coords, const Value<CoordType>& gradX, const Value<CoordType>& gradY);
	Value<ValueType> SampleGradOffset(const Value<CoordType>& coords, const Value<CoordType>& gradX, const Value<CoordType>& gradY, const Value<IntCoordType>& offset);
};

/// Cube sampler expression.
template <typename ValueType>
class SamplerCube : public SamplerBase
{
public:
	SamplerCube(int slot);

	Value<ValueType> Sample(Value<vec3> coords);
};

END_INANITY_SHADERS

#endif
