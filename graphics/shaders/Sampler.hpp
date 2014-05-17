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

/// Класс выражения-семплера (обыкновенного, 1D, 2D или 3D).
template <typename ValueType, int dimensions>
class Sampler : public SamplerBase
{
public:
	typedef typename generalxvec<float, dimensions>::Type CoordType;
	typedef typename generalxvec<int, dimensions>::Type IntCoordType;

private:
	/// Оператор присваивания запрещён (приватный).
	void operator=(Sampler a);

public:
	Sampler(ptr<SamplerNode> node);
	Sampler(int slot);

	Value<ValueType> Sample(Value<CoordType> coords);
	Value<ValueType> SampleOffset(Value<CoordType> coords, Value<IntCoordType> offset);
	Value<ValueType> SampleLod(Value<CoordType> coords, Value<float> lod);
	Value<ValueType> SampleLodOffset(Value<CoordType> coords, Value<float> lod, Value<IntCoordType> offset);
	Value<ValueType> SampleBias(Value<CoordType> coords, Value<float> bias);
	Value<ValueType> SampleBiasOffset(Value<CoordType> coords, Value<float> bias, Value<IntCoordType> offset);
	Value<ValueType> SampleGrad(Value<CoordType> coords, Value<CoordType> gradX, Value<CoordType> gradY);
	Value<ValueType> SampleGradOffset(Value<CoordType> coords, Value<CoordType> gradX, Value<CoordType> gradY, Value<IntCoordType> offset);
};

/// Класс выражения-семплера для кубического семплера.
template <typename ValueType>
class SamplerCube : public SamplerBase
{
private:
	/// Оператор присваивания запрещён (приватный).
	void operator=(SamplerCube a);

public:
	SamplerCube(ptr<SamplerNode> node);
	SamplerCube(int slot);

	/// Получить семпл.
	Value<ValueType> Sample(Value<vec3> coords);
};

END_INANITY_SHADERS

#endif
