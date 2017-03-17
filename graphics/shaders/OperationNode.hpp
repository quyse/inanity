#ifndef ___INANITY_GRAPHICS_SHADERS_OPERATION_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_OPERATION_NODE_HPP___

#include "ValueNode.hpp"

BEGIN_INANITY_SHADERS

class OperationNode : public ValueNode
{
public:
	static const int maxArgumentsCount = 4;

	enum Operation
	{
		operationNegate,
		operationAdd,
		operationSubtract,
		operationMultiply,
		operationDivide,
		operationLess,
		operationLessEqual,
		operationEqual,
		operationNotEqual,
		operationGetInstanceID,
		operationScreenToTexture,
		operationFloat11to2,
		operationFloat111to3,
		operationFloat1111to4,
		operationFloat31to4,
		operationFloat211to4,
		operationInt11to2,
		operationDot,
		operationCross,
		operationMul,
		operationLength,
		operationNormalize,
		operationLerp,
		operationPow,
		operationMin,
		operationMax,
		operationAbs,
		operationSqrt,
		operationSin,
		operationCos,
		operationAtan2,
		operationExp,
		operationExp2,
		operationLog,
		operationSaturate,
		operationDdx,
		operationDdy,
		operationFloor,
		operationCeil,
		operationMod
	};

private:
	Operation operation;
	DataType valueType;
	int argumentsCount;
	/// Arguments.
	ptr<ValueNode> a, b, c, d;

public:
	OperationNode(Operation operation, DataType valueType);
	OperationNode(Operation operation, DataType valueType, ptr<ValueNode> a);
	OperationNode(Operation operation, DataType valueType, ptr<ValueNode> a, ptr<ValueNode> b);
	OperationNode(Operation operation, DataType valueType, ptr<ValueNode> a, ptr<ValueNode> b, ptr<ValueNode> c);
	OperationNode(Operation operation, DataType valueType, ptr<ValueNode> a, ptr<ValueNode> b, ptr<ValueNode> c, ptr<ValueNode> d);

	Type GetType() const;
	DataType GetValueType() const;

	Operation GetOperation() const;
	int GetArgumentsCount() const;

	ptr<ValueNode> GetA() const;
	ptr<ValueNode> GetB() const;
	ptr<ValueNode> GetC() const;
	ptr<ValueNode> GetD() const;

	ptr<ValueNode> GetArgument(int number) const;
};

END_INANITY_SHADERS

#endif
