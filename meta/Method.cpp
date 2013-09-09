#include "Method.hpp"

BEGIN_INANITY_META

MethodBase::MethodBase(const char* name) : name(name) {}

const char* MethodBase::GetName() const
{
	return name;
}

END_INANITY_META
