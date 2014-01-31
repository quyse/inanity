#include "MetaProvider.ipp"
#include "../../meta/ClassBase.ipp"

BEGIN_INANITY_V8

//*** class MetaProvider::NamedCallableBase

MetaProvider::NamedCallableBase::NamedCallableBase(const char* name)
: name(name) {}

const char* MetaProvider::NamedCallableBase::GetName() const
{
	return name;
}

//*** class MetaProvider::ClassBase

MetaProvider::ClassBase::ClassBase(const char* name, const char* fullName)
: Meta::ClassBase<Traits>(name, fullName) {}

// Ensure we have all methods of Meta::ClassBase for linking.

template Meta::ClassBase<MetaProvider::Traits>;

END_INANITY_V8
