#include "OperationNode.hpp"
#include "../../Exception.hpp"

OperationNode::OperationNode(Operation operation)
: operation(operation), argumentsCount(0) {}
OperationNode::OperationNode(Operation operation, ptr<Node> a)
: operation(operation), argumentsCount(1), a(a) {}
OperationNode::OperationNode(Operation operation, ptr<Node> a, ptr<Node> b)
: operation(operation), argumentsCount(2), a(a), b(b) {}
OperationNode::OperationNode(Operation operation, ptr<Node> a, ptr<Node> b, ptr<Node> c)
: operation(operation), argumentsCount(3), a(a), b(b), c(c) {}
OperationNode::OperationNode(Operation operation, ptr<Node> a, ptr<Node> b, ptr<Node> c, ptr<Node> d)
: operation(operation), argumentsCount(4), a(a), b(b), c(c), d(d) {}

Node::Type OperationNode::GetType() const
{
	return typeOperation;
}

OperationNode::Operation OperationNode::GetOperation() const
{
	return operation;
}

int OperationNode::GetArgumentsCount() const
{
	return argumentsCount;
}

ptr<Node> OperationNode::GetA() const
{
	if(!a)
		THROW_PRIMARY_EXCEPTION("Argument A is unavailable");
	return a;
}

ptr<Node> OperationNode::GetB() const
{
	if(!b)
		THROW_PRIMARY_EXCEPTION("Argument B is unavailable");
	return b;
}

ptr<Node> OperationNode::GetC() const
{
	if(!c)
		THROW_PRIMARY_EXCEPTION("Argument C is unavailable");
	return c;
}

ptr<Node> OperationNode::GetD() const
{
	if(!d)
		THROW_PRIMARY_EXCEPTION("Argument D is unavailable");
	return d;
}

ptr<Node> OperationNode::GetArgument(int number) const
{
	switch(number)
	{
	case 0:
		return GetA();
	case 1:
		return GetB();
	case 2:
		return GetC();
	case 3:
		return GetD();
	default:
		THROW_PRIMARY_EXCEPTION("Invalid argument number for operation node");
	}
}
