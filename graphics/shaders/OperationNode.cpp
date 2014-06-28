#include "OperationNode.hpp"
#include "../../Exception.hpp"

BEGIN_INANITY_SHADERS

OperationNode::OperationNode(Operation operation, DataType valueType)
: operation(operation), valueType(valueType), argumentsCount(0) {}
OperationNode::OperationNode(Operation operation, DataType valueType, ptr<ValueNode> a)
: operation(operation), valueType(valueType), argumentsCount(1), a(a) {}
OperationNode::OperationNode(Operation operation, DataType valueType, ptr<ValueNode> a, ptr<ValueNode> b)
: operation(operation), valueType(valueType), argumentsCount(2), a(a), b(b) {}
OperationNode::OperationNode(Operation operation, DataType valueType, ptr<ValueNode> a, ptr<ValueNode> b, ptr<ValueNode> c)
: operation(operation), valueType(valueType), argumentsCount(3), a(a), b(b), c(c) {}
OperationNode::OperationNode(Operation operation, DataType valueType, ptr<ValueNode> a, ptr<ValueNode> b, ptr<ValueNode> c, ptr<ValueNode> d)
: operation(operation), valueType(valueType), argumentsCount(4), a(a), b(b), c(c), d(d) {}

Node::Type OperationNode::GetType() const
{
	return typeOperation;
}

DataType OperationNode::GetValueType() const
{
	return valueType;
}

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
	THROW_ASSERT(a);
	return a;
}

ptr<ValueNode> OperationNode::GetB() const
{
	THROW_ASSERT(b);
	return b;
}

ptr<ValueNode> OperationNode::GetC() const
{
	THROW_ASSERT(c);
	return c;
}

ptr<ValueNode> OperationNode::GetD() const
{
	THROW_ASSERT(d);
	return d;
}

ptr<ValueNode> OperationNode::GetArgument(int number) const
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
		THROW("Invalid argument number for operation node");
	}
}

END_INANITY_SHADERS
