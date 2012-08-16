#ifndef ___INANITY_GRAPHICS_SHADERS_EXPRESSION_HPP___
#define ___INANITY_GRAPHICS_SHADERS_EXPRESSION_HPP___

#include "shaders.hpp"

BEGIN_INANITY_SHADERS

/// Класс выражения.
template <DataType>
class Expression
{
private:
	ptr<ExpressionObject<DataType> > object;

	//******* Различные операторы для выражений.
	friend Expression<DataType> operator-(Expression<DataType> a)
	{
		return NEW(NegateExpressionObject<DataType>(a));
	}
	template <typename DataTypeA, typename DataTypeB>
	friend auto operator+(Expression<DataTypeA> a, Expression<DataTypeB> b) -> Expression<decltype(ExpressionObject<DataTypeA> + ExpressionObject<DataTypeB>)>
	{
		return NEW(AddExpressionObject<DataTypeA, DataTypeB>(a, b));
	}
	template <typename DataTypeA, typename DataTypeB>
	friend auto operator-(Expression<DataTypeA> a, Expression<DataTypeB> b) -> Expression<decltype(ExpressionObject<DataTypeA> - ExpressionObject<DataTypeB>)>
	{
		return NEW(SubtractExpressionObject<DataTypeA, DataTypeB>(a, b));
	}
	template <typename DataTypeA, typename DataTypeB>
	friend auto operator*(Expression<DataTypeA> a, Expression<DataTypeB> b) -> Expression<decltype(ExpressionObject<DataTypeA> * ExpressionObject<DataTypeB>)>
	{
		return NEW(MultiplyExpressionObject<DataTypeA, DataTypeB>(a, b));
	}
	template <typename DataTypeA, typename DataTypeB>
	friend auto operator/(Expression<DataTypeA> a, Expression<DataTypeB> b) -> Expression<decltype(ExpressionObject<DataTypeA> / ExpressionObject<DataTypeB>)>
	{
		return NEW(DivideExpressionObject<DataTypeA, DataTypeB>(a, b));
	}

public:
	Expression(ptr<ExpressionObject<DataType> > object) : object(object) {}
};

END_INANITY_SHADERS

#endif
