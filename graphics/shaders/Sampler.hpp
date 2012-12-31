#ifndef ___INANITY_GRAPHICS_SHADERS_SAMPLER_HPP___
#define ___INANITY_GRAPHICS_SHADERS_SAMPLER_HPP___

#include "Expression.hpp"

BEGIN_INANITY_SHADERS

class SamplerNode;

/// Класс выражения-семплера.
template <typename ValueType, typename CoordType>
class Sampler : public Expression
{
private:
	/// Оператор присваивания запрещён (приватный).
	void operator=(Sampler a);

public:
	Sampler(ptr<SamplerNode> node);
	Sampler(int slot);

	/// Получить семпл.
	Value<ValueType> Sample(Value<CoordType> coords);

	/// Установить текстуру в семплер.
	void SetTexture(ptr<Texture> texture);
	/// Установить настройки семплера в семплер.
	void SetSamplingState(ptr<SamplerState> samplingState);
};

END_INANITY_SHADERS

#endif
