#ifndef ___INANITY_GRAPHICS_SHADERS_UNIFORM_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_UNIFORM_NODE_HPP___

#include "Node.hpp"

BEGIN_INANITY_SHADERS

class UniformGroup;

/// Класс узла Uniform-переменной.
class UniformNode : public Node
{
private:
	ptr<UniformGroup> group;
	DataType valueType;
	int offset;

public:
	UniformNode(ptr<UniformGroup> buffer, DataType valueType, int offset);

	Type GetType() const;

	ptr<UniformGroup> GetGroup() const;
	DataType GetValueType() const;
	int GetOffset() const;
};

END_INANITY_SHADERS

#endif
