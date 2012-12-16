#ifndef ___INANITY_GRAPHICS_SHADERS_UNIFORM_HPP___
#define ___INANITY_GRAPHICS_SHADERS_UNIFORM_HPP___

#include "LValue.hpp"

BEGIN_INANITY_SHADERS

template <typename ValueType>
class Uniform : public LValue<ValueType>
{
public:
	Uniform(ptr<UniformNode> node);

	void SetValue(ValueType value);
};

END_INANITY_SHADERS

#endif
