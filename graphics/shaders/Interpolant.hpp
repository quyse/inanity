#ifndef ___INANITY_GRAPHICS_SHADERS_INTERPOLANT_HPP___
#define ___INANITY_GRAPHICS_SHADERS_INTERPOLANT_HPP___

#include "Value.hpp"

BEGIN_INANITY_SHADERS

/// Вспомогательный класс для создания интерполируемых переменных.
template <typename ValueType>
class Interpolant : public Value<ValueType>
{
private:
	int semantic;

public:
	Interpolant(int semantic);

	Expression Set(const Value<ValueType>& a);
};

END_INANITY_SHADERS

#endif
