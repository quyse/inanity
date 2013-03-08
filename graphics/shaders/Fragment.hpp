#ifndef ___INANITY_GRAPHICS_SHADERS_FRAGMENT_HPP___
#define ___INANITY_GRAPHICS_SHADERS_FRAGMENT_HPP___

#include "LValue.hpp"

BEGIN_INANITY_SHADERS

/// Вспомогательный класс для определения переменной - выхода пиксельного шейдера.
template <typename ValueType>
class Fragment : public LValue<ValueType>
{
public:
	Fragment(int semantic);

	/// Явный оператор присваивания.
	Value<ValueType> operator=(Value<ValueType> a);
};

END_INANITY_SHADERS

#endif
