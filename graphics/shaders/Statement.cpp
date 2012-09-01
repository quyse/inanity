#include "Statement.hpp"
#include "StatementObjects.hpp"
#include "ExpressionObject.hpp"

Statement::Statement(ptr<StatementObject> object) : object(object) {}

Statement::Statement(Expression expression)
: object(NEW(ExpressionStatementObject(expression))) {}

BEGIN_INANITY_SHADERS

Statement operator,(Statement a, Statement b)
{
	return NEW(CommaStatementObject(a, b));
}

END_INANITY_SHADERS
