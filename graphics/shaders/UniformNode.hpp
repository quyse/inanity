#ifndef ___INANITY_GRAPHICS_SHADERS_UNIFORM_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_UNIFORM_NODE_HPP___

#include "Node.hpp"

BEGIN_INANITY_SHADERS

/// Класс узла Uniform-переменной.
class UniformNode : public Node
{
private:
	ptr<UniformBuffer> buffer;
	int offset;
	DataType valueType;

public:
	UniformNode(ptr<UniformBuffer> buffer, int offset, DataType valueType);

	ptr<UniformBuffer> GetBuffer() const;
	int GetOffset() const;
	DataType GetValueType() const;
};

END_INANITY_SHADERS

#endif
