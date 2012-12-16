#include "OperationNode.hpp"

OperationNode::OperationNode(Operation operation)
: operation(operation), argumentsCount(0) {}
OperationNode::OperationNode(Operation operation, ptr<ValueNode> a)
: operation(operation), argumentsCount(1), a(a) {}
OperationNode::OperationNode(Operation operation, ptr<ValueNode> a, ptr<ValueNode> b)
: operation(operation), argumentsCount(2), a(a), b(b) {}
OperationNode::OperationNode(Operation operation, ptr<ValueNode> a, ptr<ValueNode> b, ptr<ValueNode> c)
: operation(operation), argumentsCount(3), a(a), b(b), c(c) {}
OperationNode::OperationNode(Operation operation, ptr<ValueNode> a, ptr<ValueNode> b, ptr<ValueNode> c, ptr<ValueNode> d)
: operation(operation), argumentsCount(4), a(a), b(b), c(c), d(d) {}

OperationNode::Operation OperationNode::GetOperation() const
{
	return operation;
}

int OperationNode::GetArgumentsCount() const
{
	return argumentsCount;
}

ptr<ValueNode> OperationNode::GetA() const
{
	return a;
}

ptr<ValueNode> OperationNode::GetB() const
{
	return b;
}

ptr<ValueNode> OperationNode::GetC() const
{
	return c;
}

ptr<ValueNode> OperationNode::GetD() const
{
	return d;
}
