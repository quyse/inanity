#include "ActionNode.hpp"
#include "ValueNode.hpp"
#include "../../Exception.hpp"

BEGIN_INANITY_SHADERS

ActionNode::ActionNode(Action action)
: action(action), argumentsCount(0) {}
ActionNode::ActionNode(Action action, ptr<ValueNode> a)
: action(action), argumentsCount(1), a(a) {}
ActionNode::ActionNode(Action action, ptr<ValueNode> a, ptr<ValueNode> b)
: action(action), argumentsCount(2), a(a), b(b) {}

Node::Type ActionNode::GetType() const
{
	return typeAction;
}

ActionNode::Action ActionNode::GetAction() const
{
	return action;
}

int ActionNode::GetArgumentsCount() const
{
	return argumentsCount;
}

ptr<ValueNode> ActionNode::GetA() const
{
	if(!a)
		THROW("Argument A is unavailable");
	return a;
}

ptr<ValueNode> ActionNode::GetB() const
{
	if(!b)
		THROW("Argument B is unavailable");
	return b;
}

ptr<ValueNode> ActionNode::GetArgument(int number) const
{
	switch(number)
	{
	case 0:
		return GetA();
	case 1:
		return GetB();
	default:
		THROW("Invalid argument number for action node");
	}
}

END_INANITY_SHADERS
