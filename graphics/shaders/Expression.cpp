#include "Expression.hpp"
#include "ExpressionObjects.hpp"

Expression::Expression(ptr<ExpressionObject> object) : object(object) {}

BEGIN_INANITY_SHADERS

Expression Expression::operator=(Expression b)
{
	return NEW(AssignExpressionObject(*this, b));
}

Expression Expression::operator[](Expression index)
{
	return NEW(SubscriptExpressionObject(*this, index));
}

Expression operator-(Expression a)
{
	return NEW(NegateExpressionObject(a));
}

Expression operator+(Expression a, Expression b)
{
	return NEW(AddExpressionObject(a, b));
}

Expression operator-(Expression a, Expression b)
{
	return NEW(SubtractExpressionObject(a, b));
}

Expression operator*(Expression a, Expression b)
{
	return NEW(MultiplyExpressionObject(a, b));
}

Expression operator/(Expression a, Expression b)
{
	return NEW(DivideExpressionObject(a, b));
}

END_INANITY_SHADERS
