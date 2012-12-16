#ifndef ___INANITY_GRAPHICS_SHADERS_OPERATION_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_OPERATION_NODE_HPP___

#include "ValueNode.hpp"

BEGIN_INANITY_SHADERS

/// Класс выражения-операции.
class OperationNode : public ValueNode
{
public:
	/// Максимальное количество аргументов.
	static const int maxArgumentsCount = 4;

	/// Операция.
	enum Operation
	{
		operationSwizzle,
		operationAssign,
		operationNegate,
		operationAdd,
		operationSubtract,
		operationMultiply,
		operationDivide,
		operationDot,
		operationCross,
	};

private:
	/// Операция.
	Operation operation;
	/// Аргументы.
	ptr<ValueNode> a, b, c, d;
	/// Количество аргументов.
	int argumentsCount;

public:
	/// Конструкторы.
	OperationNode(Operation operation);
	OperationNode(Operation operation, ptr<ValueNode> a);
	OperationNode(Operation operation, ptr<ValueNode> a, ptr<ValueNode> b);
	OperationNode(Operation operation, ptr<ValueNode> a, ptr<ValueNode> b, ptr<ValueNode> c);
	OperationNode(Operation operation, ptr<ValueNode> a, ptr<ValueNode> b, ptr<ValueNode> c, ptr<ValueNode> d);

	Operation GetOperation() const;
	int GetArgumentsCount() const;

	ptr<ValueNode> GetA() const;
	ptr<ValueNode> GetB() const;
	ptr<ValueNode> GetC() const;
	ptr<ValueNode> GetD() const;
};

END_INANITY_SHADERS

#endif
