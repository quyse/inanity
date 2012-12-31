#ifndef ___INANITY_GRAPHICS_SHADERS_UNIFORM_HPP___
#define ___INANITY_GRAPHICS_SHADERS_UNIFORM_HPP___

#include "Value.hpp"

BEGIN_INANITY_SHADERS

class UniformNode;

template <typename ValueType>
class Uniform : public Value<ValueType>
{
public:
	Uniform(ptr<UniformNode> node);

	void SetValue(const ValueType& value);
};

END_INANITY_SHADERS

#endif
