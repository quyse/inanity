#include "Function.ipp"

BEGIN_INANITY_META

FunctionBase::FunctionBase(const char* name) : name(name) {}

const char* FunctionBase::GetName() const
{
	return name;
}

END_INANITY_META
