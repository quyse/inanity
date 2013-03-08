#ifndef ___INANITY_GRAPHICS_SHADERS_SAMPLER_HPP___
#define ___INANITY_GRAPHICS_SHADERS_SAMPLER_HPP___

#include "Expression.hpp"
#include "Value.hpp"

BEGIN_INANITY_GRAPHICS

class Texture;
class SamplerState;
struct ContextState;

END_INANITY_GRAPHICS

BEGIN_INANITY_SHADERS

class SamplerNode;

class SamplerBase : public Expression
{
private:
	ptr<Texture> texture;
	ptr<SamplerState> samplerState;

protected:
	SamplerBase(ptr<SamplerNode> node);

public:
	/// Установить текстуру в семплер.
	void SetTexture(ptr<Texture> texture);
	/// Установить настройки семплера в семплер.
	void SetSamplerState(ptr<SamplerState> samplerState);

	/// Применить семплер к состоянию.
	void Apply(ContextState& contextState) const;
};

/// Класс выражения-семплера.
template <typename ValueType, typename CoordType>
class Sampler : public SamplerBase
{
private:
	/// Оператор присваивания запрещён (приватный).
	void operator=(Sampler a);

public:
	Sampler(ptr<SamplerNode> node);
	Sampler(int slot);

	/// Получить семпл.
	Value<ValueType> Sample(Value<CoordType> coords);
};

END_INANITY_SHADERS

#endif
