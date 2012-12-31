#ifndef ___INANITY_GRAPHICS_SHADERS_SEQUENCE_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_SEQUENCE_NODE_HPP___

#include "Node.hpp"

BEGIN_INANITY_SHADERS

/// Класс узла, соединяющий несколько узлов в последовательность.
class SequenceNode : public Node
{
private:
	ptr<Node> a, b;

public:
	SequenceNode(ptr<Node> a, ptr<Node> b);

	Type GetType() const;

	ptr<Node> GetA() const;
	ptr<Node> GetB() const;
};

END_INANITY_SHADERS

#endif
