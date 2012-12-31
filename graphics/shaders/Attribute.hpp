#ifndef ___INANITY_GRAPHICS_SHADERS_ATTRIBUTE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_ATTRIBUTE_HPP___

#include "Value.hpp"

BEGIN_INANITY_SHADERS

/// Вспомогательный класс для определения атрибутов.
template <typename ValueType>
class Attribute : public Value<ValueType>
{
public:
	Attribute(int layoutSemantic);
};

END_INANITY_SHADERS

#endif
