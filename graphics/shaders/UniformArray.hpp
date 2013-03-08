#ifndef ___INANITY_GRAPHICS_SHADERS_UNIFORM_ARRAY_HPP___
#define ___INANITY_GRAPHICS_SHADERS_UNIFORM_ARRAY_HPP___

#include "Value.hpp"

BEGIN_INANITY_SHADERS

class UniformNode;

template <typename ValueType>
class UniformArray : public Value<ValueType>
{
public:
	UniformArray(ptr<UniformNode> node);

	/// Операция индексирования массива.
	Value<ValueType> operator[](Value<unsigned int> index) const;

	void SetValue(int index, const ValueType& value);
};

END_INANITY_SHADERS

#endif
