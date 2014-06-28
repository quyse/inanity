#ifndef ___INANITY_GRAPHICS_SHADERS_UNIFORM_HPP___
#define ___INANITY_GRAPHICS_SHADERS_UNIFORM_HPP___

#include "Value.hpp"

BEGIN_INANITY_SHADERS

class UniformNode;

template <typename ValueType>
class Uniform : public Value<ValueType>
{
private:
	ptr<UniformNode> uniformNode;

public:
	Uniform(ptr<UniformNode> uniformNode = nullptr);

	void Set(const ValueType& value);
};

END_INANITY_SHADERS

#endif
