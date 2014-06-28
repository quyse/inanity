#ifndef ___INANITY_GRAPHICS_SHADERS_ACTION_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_ACTION_NODE_HPP___

#include "Node.hpp"

BEGIN_INANITY_SHADERS

class ValueNode;

class ActionNode : public Node
{
public:
	static const int maxArgumentsCount = 2;

	enum Action
	{
		actionSetPosition,
		actionClip
	};

private:
	Action action;
	int argumentsCount;
	ptr<ValueNode> a, b;

public:
	ActionNode(Action action);
	ActionNode(Action action, ptr<ValueNode> a);
	ActionNode(Action action, ptr<ValueNode> a, ptr<ValueNode> b);

	Type GetType() const;

	Action GetAction() const;
	int GetArgumentsCount() const;

	ptr<ValueNode> GetA() const;
	ptr<ValueNode> GetB() const;

	ptr<ValueNode> GetArgument(int number) const;
};

END_INANITY_SHADERS

#endif
