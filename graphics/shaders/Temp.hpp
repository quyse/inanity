#ifndef ___INANITY_GRAPHICS_SHADERS_TEMP_HPP___
#define ___INANITY_GRAPHICS_SHADERS_TEMP_HPP___

#include "LValue.hpp"

BEGIN_INANITY_SHADERS

/// Вспомогательный класс для создания временных переменных.
template <typename ValueType>
class Temp : public LValue<ValueType>
{
public:
	Temp();

	/// Явный оператор присваивания.
	Value<ValueType> operator=(Value<ValueType> a);
};

END_INANITY_SHADERS

#endif
