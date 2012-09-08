#ifndef ___INANITY_GRAPHICS_SHADERS_EXPRESSION_HPP___
#define ___INANITY_GRAPHICS_SHADERS_EXPRESSION_HPP___

#include "shaders.hpp"

BEGIN_INANITY_SHADERS

class ExpressionObject;

/// Класс выражения.
class Expression
{
private:
	ptr<ExpressionObject> object;

	//******* Различные операторы для выражений.
	Expression operator=(Expression b);
	Expression operator[](Expression index);
	friend Expression operator-(Expression a);
	friend Expression operator+(Expression a, Expression b);
	friend Expression operator-(Expression a, Expression b);
	friend Expression operator*(Expression a, Expression b);
	friend Expression operator/(Expression a, Expression b);

public:
	Expression(ptr<ExpressionObject> object);

	ptr<ExpressionObject> GetObject() const;
};

END_INANITY_SHADERS

#endif
