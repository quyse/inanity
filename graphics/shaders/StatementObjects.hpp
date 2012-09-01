#ifndef ___INANITY_GRAPHICS_SHADERS_STATEMENT_OBJECTS_HPP___
#define ___INANITY_GRAPHICS_SHADERS_STATEMENT_OBJECTS_HPP___

#include "StatementObject.hpp"
#include "Expression.hpp"
#include "Statement.hpp"

BEGIN_INANITY_SHADERS

/// Класс Statementent для конвертирования expression в statement.
class ExpressionStatementObject : public StatementObject
{
private:
	Expression expression;

public:
	ExpressionStatementObject(Expression expression);

	Type GetType() const;

	Expression GetExpression() const;
};

/// Класс для statement'а, состоящего из двух statement'ов.
class CommaStatementObject : public StatementObject
{
private:
	Statement a;
	Statement b;

public:
	CommaStatementObject(Statement a, Statement b);

	Type GetType() const;

	Statement GetLeft() const;
	Statement GetRight() const;
};

/// Statement оператора if.
class IfStatementObject : public StatementObject
{
private:
	Expression condition;
	Statement trueBody;
	Statement falseBody;

public:
	IfStatementObject(Expression condition, Statement trueBody, Statement falseBody);

	Type GetType() const;

	Expression GetCondition() const;
	Statement GetTrueBody() const;
	Statement GetFalseBody() const;
};

END_INANITY_SHADERS

#endif
