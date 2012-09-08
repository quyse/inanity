#include "ExpressionObjects.hpp"

//******* VariableExpressionObject

VariableExpressionObject::VariableExpressionObject(int offset) : offset(offset) {}

int VariableExpressionObject::GetOffset() const
{
	return offset;
}

//******* InExpressionObject

InExpressionObject::InExpressionObject(int offset) : VariableExpressionObject(offset) {}

ExpressionObject::Type InExpressionObject::GetType() const
{
	return typeIn;
}

//******* OutExpressionObject

OutExpressionObject::OutExpressionObject(int offset) : VariableExpressionObject(offset) {}

ExpressionObject::Type OutExpressionObject::GetType() const
{
	return typeOut;
}

//******* UniformExpressionObject

UniformExpressionObject::UniformExpressionObject(int buffer, int offset)
: buffer(buffer), offset(offset) {}

ExpressionObject::Type UniformExpressionObject::GetType() const
{
	return typeUniform;
}

int UniformExpressionObject::GetBuffer() const
{
	return buffer;
}

int UniformExpressionObject::GetOffset() const
{
	return offset;
}

//******* SamplerExpressionObject

SamplerExpressionObject::SamplerExpressionObject(int slot)
: slot(slot) {}

ExpressionObject::Type SamplerExpressionObject::GetType() const
{
	return typeSampler;
}

int SamplerExpressionObject::GetSlot() const
{
	return slot;
}

//******* TempExpressionObject

TempExpressionObject::TempExpressionObject(int offset) : VariableExpressionObject(offset) {}

ExpressionObject::Type TempExpressionObject::GetType() const
{
	return typeTemp;
}

//******* UnaryExpressionObject

UnaryExpressionObject::UnaryExpressionObject(Expression a) : a(a) {}

Expression UnaryExpressionObject::GetInner() const
{
	return a;
}

//******* NegateExpressionObject

NegateExpressionObject::NegateExpressionObject(Expression a) : UnaryExpressionObject(a) {}

ExpressionObject::Type NegateExpressionObject::GetType() const
{
	return typeNegate;
}

//******* BinaryExpressionObject

BinaryExpressionObject::BinaryExpressionObject(Expression a, Expression b) : a(a), b(b) {}

Expression BinaryExpressionObject::GetLeft() const
{
	return a;
}

Expression BinaryExpressionObject::GetRight() const
{
	return b;
}

//******* AssignExpressionObject

AssignExpressionObject::AssignExpressionObject(Expression a, Expression b) : BinaryExpressionObject(a, b) {}

ExpressionObject::Type AssignExpressionObject::GetType() const
{
	return typeAssign;
}

//******* SubscriptExpressionObject

SubscriptExpressionObject::SubscriptExpressionObject(Expression a, Expression b) : BinaryExpressionObject(a, b) {}

ExpressionObject::Type SubscriptExpressionObject::GetType() const
{
	return typeSubscript;
}

//******* AddExpressionObject

AddExpressionObject::AddExpressionObject(Expression a, Expression b) : BinaryExpressionObject(a, b) {}

ExpressionObject::Type AddExpressionObject::GetType() const
{
	return typeAdd;
}

//******* SubtractExpressionObject

SubtractExpressionObject::SubtractExpressionObject(Expression a, Expression b) : BinaryExpressionObject(a, b) {}

ExpressionObject::Type SubtractExpressionObject::GetType() const
{
	return typeSubtract;
}

//******* MultiplyExpressionObject

MultiplyExpressionObject::MultiplyExpressionObject(Expression a, Expression b) : BinaryExpressionObject(a, b) {}

ExpressionObject::Type MultiplyExpressionObject::GetType() const
{
	return typeMultiply;
}

//******* DivideExpressionObject

DivideExpressionObject::DivideExpressionObject(Expression a, Expression b) : BinaryExpressionObject(a, b) {}

ExpressionObject::Type DivideExpressionObject::GetType() const
{
	return typeDivide;
}

//******* CallExpressionObject

CallExpressionObject::CallExpressionObject(const char* name) : name(name) {}

const char* CallExpressionObject::GetName() const
{
	return name;
}

//******* Call1ExpressionObject

Call1ExpressionObject::Call1ExpressionObject(const char* name, Expression a)
: CallExpressionObject(name), a(a) {}

ExpressionObject::Type Call1ExpressionObject::GetType() const
{
	return typeCall1;
}

Expression Call1ExpressionObject::GetArg1() const
{
	return a;
}

//******* Call2ExpressionObject

Call2ExpressionObject::Call2ExpressionObject(const char* name, Expression a, Expression b)
: CallExpressionObject(name), a(a), b(b) {}

ExpressionObject::Type Call2ExpressionObject::GetType() const
{
	return typeCall2;
}

Expression Call2ExpressionObject::GetArg1() const
{
	return a;
}

Expression Call2ExpressionObject::GetArg2() const
{
	return b;
}

//******* Call3ExpressionObject

Call3ExpressionObject::Call3ExpressionObject(const char* name, Expression a, Expression b, Expression c)
: CallExpressionObject(name), a(a), b(b), c(c) {}

ExpressionObject::Type Call3ExpressionObject::GetType() const
{
	return typeCall3;
}

Expression Call3ExpressionObject::GetArg1() const
{
	return a;
}

Expression Call3ExpressionObject::GetArg2() const
{
	return b;
}

Expression Call3ExpressionObject::GetArg3() const
{
	return c;
}
