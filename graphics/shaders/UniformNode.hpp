#ifndef ___INANITY_GRAPHICS_SHADERS_UNIFORM_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_UNIFORM_NODE_HPP___

#include "Node.hpp"
#include "../DataType.hpp"

BEGIN_INANITY_SHADERS

class UniformGroup;

/// Uniform variable.
class UniformNode : public Node
{
private:
	ptr<UniformGroup> group;
	DataType valueType;
	int offset;
	int count;

public:
	UniformNode(ptr<UniformGroup> group, DataType valueType, int offset, int count);

	Type GetType() const;

	ptr<UniformGroup> GetGroup() const;
	DataType GetValueType() const;
	int GetOffset() const;
	int GetCount() const;
};

END_INANITY_SHADERS

#endif
