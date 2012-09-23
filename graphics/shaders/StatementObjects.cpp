#include "StatementObjects.hpp"
#include "ExpressionObject.hpp"

ExpressionStatementObject::ExpressionStatementObject(Expression expression)
: expression(expression) {}

StatementObject::Type ExpressionStatementObject::GetType() const
{
	return typeExpression;
}

Expression ExpressionStatementObject::GetExpression() const
{
	return expression;
}

CommaStatementObject::CommaStatementObject(Statement a, Statement b)
: a(a), b(b) {}

StatementObject::Type CommaStatementObject::GetType() const
{
	return typeComma;
}

Statement CommaStatementObject::GetLeft() const
{
	return a;
}

Statement CommaStatementObject::GetRight() const
{
	return b;
}

IfStatementObject::IfStatementObject(Expression condition, Statement trueBody, Statement falseBody)
: condition(condition), trueBody(trueBody), falseBody(falseBody) {}

StatementObject::Type IfStatementObject::GetType() const
{
	return typeIf;
}

Expression IfStatementObject::GetCondition() const
{
	return condition;
}

Statement IfStatementObject::GetTrueBody() const
{
	return trueBody;
}

Statement IfStatementObject::GetFalseBody() const
{
	return falseBody;
}
