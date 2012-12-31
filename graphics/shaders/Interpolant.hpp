#ifndef ___INANITY_GRAPHICS_SHADERS_INTERPOLANT_HPP___
#define ___INANITY_GRAPHICS_SHADERS_INTERPOLANT_HPP___

#include "LValue.hpp"

BEGIN_INANITY_SHADERS

/// Вспомогательный класс для создания интерполируемых переменных.
template <typename ValueType>
class Interpolant : public LValue<ValueType>
{
public:
	Interpolant(Semantic semantic);

	/// Явный оператор присваивания.
	Value<ValueType> operator=(Value<ValueType> a);
};

END_INANITY_SHADERS

#endif
