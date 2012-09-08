#ifndef ___INANITY_GRAPHICS_SHADERS_EXPRESSION_OBJECTS_HPP___
#define ___INANITY_GRAPHICS_SHADERS_EXPRESSION_OBJECTS_HPP___

#include "ExpressionObject.hpp"
#include "Expression.hpp"

/* Файл содержит классы конкретных выражений.
*/

BEGIN_INANITY_SHADERS

class VariableExpressionObject : public ExpressionObject
{
private:
	int offset;

protected:
	VariableExpressionObject(int offset);

public:
	int GetOffset() const;
};

class InExpressionObject : public VariableExpressionObject
{
public:
	InExpressionObject(int offset);

	Type GetType() const;
};

class OutExpressionObject : public VariableExpressionObject
{
public:
	OutExpressionObject(int offset);

	Type GetType() const;
};

class UniformExpressionObject : public ExpressionObject
{
private:
	int buffer;
	int offset;

public:
	UniformExpressionObject(int buffer, int offset);

	Type GetType() const;

	int GetBuffer() const;
	int GetOffset() const;
};

class SamplerExpressionObject : public ExpressionObject
{
private:
	int slot;

public:
	SamplerExpressionObject(int slot);

	Type GetType() const;

	int GetSlot() const;
};

class TempExpressionObject : public VariableExpressionObject
{
public:
	TempExpressionObject(int offset);

	Type GetType() const;
};

class UnaryExpressionObject : public ExpressionObject
{
private:
	Expression a;

protected:
	UnaryExpressionObject(Expression a);

public:
	Expression GetInner() const;
};

class NegateExpressionObject : public UnaryExpressionObject
{
public:
	NegateExpressionObject(Expression a);

	Type GetType() const;
};

class BinaryExpressionObject : public ExpressionObject
{
private:
	Expression a;
	Expression b;

protected:
	BinaryExpressionObject(Expression a, Expression b);

public:
	Expression GetLeft() const;
	Expression GetRight() const;
};

class AssignExpressionObject : public BinaryExpressionObject
{
public:
	AssignExpressionObject(Expression a, Expression b);

	Type GetType() const;
};

class SubscriptExpressionObject : public BinaryExpressionObject
{
public:
	SubscriptExpressionObject(Expression a, Expression b);

	Type GetType() const;
};

class AddExpressionObject : public BinaryExpressionObject
{
public:
	AddExpressionObject(Expression a, Expression b);

	Type GetType() const;
};

class SubtractExpressionObject : public BinaryExpressionObject
{
public:
	SubtractExpressionObject(Expression a, Expression b);

	Type GetType() const;
};

class MultiplyExpressionObject : public BinaryExpressionObject
{
public:
	MultiplyExpressionObject(Expression a, Expression b);

	Type GetType() const;
};

class DivideExpressionObject : public BinaryExpressionObject
{
public:
	DivideExpressionObject(Expression a, Expression b);

	Type GetType() const;
};

class CallExpressionObject : public ExpressionObject
{
protected:
	const char* name;

public:
	CallExpressionObject(const char* name);

	const char* GetName() const;
};

class Call1ExpressionObject : public CallExpressionObject
{
private:
	Expression a;

public:
	Call1ExpressionObject(const char* name, Expression a);

	Type GetType() const;

	Expression GetArg1() const;
};

class Call2ExpressionObject : public CallExpressionObject
{
private:
	Expression a;
	Expression b;

public:
	Call2ExpressionObject(const char* name, Expression a, Expression b);

	Type GetType() const;

	Expression GetArg1() const;
	Expression GetArg2() const;
};

class Call3ExpressionObject : public CallExpressionObject
{
private:
	Expression a;
	Expression b;
	Expression c;

public:
	Call3ExpressionObject(const char* name, Expression a, Expression b, Expression c);

	Type GetType() const;

	Expression GetArg1() const;
	Expression GetArg2() const;
	Expression GetArg3() const;
};

END_INANITY_SHADERS

#endif
