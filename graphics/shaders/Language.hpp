#ifndef ___INANITY_GRAPHICS_SHADERS_LANGUAGE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_LANGUAGE_HPP___

#include "Language_decl.hpp"

BEGIN_INANITY_SHADERS

/// Класс statement.
/** По сути, это разновидность умного указателя на внутренний объект StatementObject.
Это сделано для того, чтобы его легко можно было бы присваивать или копировать,
и при этом использовать дополнительные операторы. */
class Statement
{
private:
	ptr<StatementObject> object;

public:
	/// Основной конструктор - из указателя на StatementObject.
	Statement(ptr<StatementObject> object) : object(object) {}

	/// Конструктор для инкапсуляции выражения в statement.
	template <typename DataType>
	Statement(Expression<DataType> expression)
	: object(NEW(ExpressionStatementObject<DataType>(expression.GetObject()))) {}

	/// Конкатенация statement'ов, для объединения их в один.
	friend const Statement operator,(const Statement& a, const Statement& b);
};

/// Абстрактный класс объекта statement.
class StatementObject : public Object
{
public:
	/// Тип statement'а.
	enum Type
	{
		typeExpression,
		typeComma,
		typeIf
	};

public:
	virtual Type GetType() const = 0;
};

/// Класс statement для конвертирования expression в statement.
template <typename DataType>
class ExpressionStatementObject : public StatementObject
{
private:
	/// Выражение.
	Expression<DataType> expression;

public:
	/// Конструктор.
	ExpressionStatementObject(Expression<DataType> expression)
	: expression(expression) {}

	Type GetType() const
	{
		return typeExpression;
	}

	Expression<DataType> GetExpression() const
	{
		return expression;
	}
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
	Expression<bool> condition;
	Statement trueBody;
	Statement falseBody;

public:
	IfStatementObject(Expression<bool> condition, Statement trueBody, Statement falseBody);

	Type GetType() const;

	Expression<bool> GetCondition() const;
	Statement GetTrueBody() const;
	Statement GetFalseBody() const;
};

/// Абстрактный класс объекта-выражения.
template <typename DataType>
class ExpressionObject : public Object
{
public:
	/// Тип выражения.
	enum Type
	{
		/// Входная переменная.
		typeIn,
		/// Выходная переменная.
		typeOut,
		/// Константная (uniform) переменная.
		typeUniform,
		/// Временная (локальная) переменная.
		typeTemp,
		/// Операция унарный минус.
		typeNegate,
		/// Сложение.
		typeAdd,
		/// Вычитание.
		typeSubtract,
		/// Умножение.
		typeMultiply,
		/// Деление.
		typeDivide,
		/// Вызов функции с одним аргументом.
		typeCall1,
		/// Вызов функции с двумя аргументами.
		typeCall2,
		/// Вызов функции с тремя аргументами.
		typeCall3
	};

public:
	virtual Type GetType() const = 0;
	const TypeInfo* GetDataTypeInfo() const { return &TypeInfoOf<DataType>::instance; }
};

template <typename DataType>
class NegateExpressionObject : public ExpressionObject<DataType>
{
public:
	typedef Expression<DataType> A;

private:
	A a;

public:
	NegateExpressionObject(A a) : a(a) {}

	Type GetType() const { return typeNegate; }
};

template <typename DataTypeA, typename DataTypeB>
class AddExpressionObject : public ExpressionObject<decltype(DataTypeA() + DataTypeB())>
{
public:
	typedef Expression<DataTypeA> A;
	typedef Expression<DataTypeB> B;

private:
	A a;
	B b;

public:
	AddExpressionObject(A a, B b) : a(a), b(b) {}

	Type GetType() const { return typeAdd; }

	A GetLeft() const { return a; }
	B GetRight() const { return b; }
};

template <typename DataTypeA, typename DataTypeB>
class SubtractExpressionObject : public ExpressionObject<decltype(DataTypeA() - DataTypeB())>
{
public:
	typedef Expression<DataTypeA> A;
	typedef Expression<DataTypeB> B;

private:
	A a;
	B b;

public:
	SubtractExpressionObject(A a, B b) : a(a), b(b) {}

	Type GetType() const { return typeSubtract; }

	A GetLeft() const { return a; }
	B GetRight() const { return b; }
};

template <typename DataTypeA, typename DataTypeB>
class MultiplyExpressionObject : public ExpressionObject<decltype(DataTypeA() * DataTypeB())>
{
public:
	typedef Expression<DataTypeA> A;
	typedef Expression<DataTypeB> B;

private:
	A a;
	B b;

public:
	MultiplyExpressionObject(A a, B b) : a(a), b(b) {}

	Type GetType() const { return typeMultiply; }

	A GetLeft() const { return a; }
	B GetRight() const { return b; }
};

template <typename DataTypeA, typename DataTypeB>
class DivideExpressionObject : public ExpressionObject<decltype(DataTypeA() / DataTypeB())>
{
public:
	typedef Expression<DataTypeA> A;
	typedef Expression<DataTypeB> B;

private:
	A a;
	B b;

public:
	DivideExpressionObject(A a, B b) : a(a), b(b) {}

	Type GetType() const { return typeDivide; }

	A GetLeft() const { return a; }
	B GetRight() const { return b; }
};

template <typename DataType>
class CallExpressionObject : public Expression<DataType>
{
protected:
	const char* name;

public:
	CallExpressionObject(const char* name) : name(name) {}

	const char* GetName() const { return name; }
};

template <typename DataType, typedef ArgTypeA>
class Call1ExpressionObject : public CallExpressionObject<DataType>
{
public:
	typedef Expression<ArgTypeA> A;

private:
	A a;

public:
	Call1ExpressionObject(const char* name, A a) : CallExpressionObject<DataType>(name), a(a) {}

	Type GetType() const { return typeCall1; }

	A GetArg1() const { return a; }
};

template <typename DataType, typedef ArgTypeA, typedef ArgTypeB>
class Call2ExpressionObject : public CallExpressionObject<DataType>
{
public:
	typedef Expression<ArgTypeA> A;
	typedef Expression<ArgTypeB> B;

private:
	A a;
	B b;

public:
	Call2ExpressionObject(const char* name, A a, B b) : CallExpressionObject<DataType>(name), a(a), b(b) {}

	Type GetType() const { return typeCall1; }

	A GetArg1() const { return a; }
	B GetArg2() const { return b; }
};

template <typename DataType, typedef ArgTypeA, typedef ArgTypeB, typedef ArgTypeC>
class Call3ExpressionObject : public CallExpressionObject<DataType>
{
public:
	typedef Expression<ArgTypeA> A;
	typedef Expression<ArgTypeB> B;
	typedef Expression<ArgTypeC> C;

private:
	A a;
	B b;
	C c;

public:
	Call3ExpressionObject(const char* name, A a, B b, C c) : CallExpressionObject<DataType>(name), a(a), b(b), c(c) {}

	Type GetType() const { return typeCall1; }

	A GetArg1() const { return a; }
	B GetArg2() const { return b; }
	C GetArg3() const { return c; }
};

/// Класс шейдера.
template <typename InputType, typename OutputType, typename UniformsType, typename TempType>
class Shader
{
private:
	Statement code;

public:
	/// Основной конструктор.
	Shader(Statement code);

	Statement GetCode() const;
};

END_INANITY_SHADERS

#endif
