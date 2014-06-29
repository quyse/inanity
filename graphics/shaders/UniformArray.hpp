#ifndef ___INANITY_GRAPHICS_SHADERS_UNIFORM_ARRAY_HPP___
#define ___INANITY_GRAPHICS_SHADERS_UNIFORM_ARRAY_HPP___

#include "Value.hpp"

BEGIN_INANITY_SHADERS

class UniformNode;

template <typename ValueType>
class UniformArray
{
private:
	ptr<UniformNode> uniformNode;

public:
	UniformArray(ptr<UniformNode> uniformNode = nullptr);

	Value<ValueType> operator[](Value<uint> index) const;

	void Set(int index, const ValueType& value);
};

END_INANITY_SHADERS

#endif
