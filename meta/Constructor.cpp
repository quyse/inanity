#include "Constructor.ipp"

BEGIN_INANITY_META

ConstructorBase::ConstructorBase(const char* name) : name(name) {}

const char* ConstructorBase::GetName() const
{
	return name;
}

END_INANITY_META
