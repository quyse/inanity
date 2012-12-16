#ifndef ___INANITY_GRAPHICS_SHADERS_SAMPLER_HPP___
#define ___INANITY_GRAPHICS_SHADERS_SAMPLER_HPP___

#include "shaders.hpp"

BEGIN_INANITY_SHADERS

class SamplerNode;

/// Класс выражения-семплера.
template <typename ValueType, typename CoordType>
class Sampler
{
protected:
	ptr<SamplerNode> node;

private:
	/// Оператор присваивания запрещён (приватный).
	void operator=(Sampler a);

public:
	Sampler(ptr<SamplerNode> node);

	ptr<SamplerNode> GetNode() const;

	/// Получить семпл.
	Value<ValueType> Sample(Value<CoordType> coords);
};

END_INANITY_SHADERS

#endif
