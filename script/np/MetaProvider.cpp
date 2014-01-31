#include "MetaProvider.ipp"

BEGIN_INANITY_NP

//*** class MetaProvider::NamedCallableBase

MetaProvider::NamedCallableBase::NamedCallableBase(const char* name)
: name(name) {}

const char* MetaProvider::NamedCallableBase::GetName() const
{
	return name;
}

//*** class MetaProvider::FunctionBase

MetaProvider::FunctionBase::FunctionBase(const char* name)
: NamedCallableBase(name) {}

//*** class MetaProvider::MethodBase

MetaProvider::MethodBase::MethodBase(const char* name)
: NamedCallableBase(name) {}

END_INANITY_NP
