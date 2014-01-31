#include "MetaProvider.ipp"
#include "../../meta/ClassBase.ipp"

BEGIN_INANITY_LUA

//*** class MetaProvider::NamedCallableBase

MetaProvider::NamedCallableBase::NamedCallableBase(const char* name)
: name(name) {}

const char* MetaProvider::NamedCallableBase::GetName() const
{
	return name;
}

// Ensure we have all methods of Meta::ClassBase for linking.

template Meta::ClassBase<MetaProvider::Traits>;

END_INANITY_LUA
