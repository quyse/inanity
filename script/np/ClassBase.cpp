#include "ClassBase.hpp"
#include "MetaProvider.ipp"
#include "../../meta/ClassBase.ipp"
#include "../../platform/NpapiPlugin.hpp"

BEGIN_INANITY_NP

ClassBase::ClassBase(const char* name, const char* fullName)
: Meta::ClassBase<MetaProvider::Traits>(name, fullName), npClassWrapper(this) {}

void ClassBase::AddMethod(MethodBase* method)
{
	methodsByIdentifier.insert(
		std::make_pair(
			Platform::NpapiPlugin::browserFuncs.getstringidentifier(
				method->GetName()),
			(int)methods.size()));
	Meta::ClassBase<MetaProvider::Traits>::AddMethod(method);
}

void ClassBase::AddStaticMethod(FunctionBase* staticMethod)
{
	staticMethodsByIdentifier.insert(
		std::make_pair(
			Platform::NpapiPlugin::browserFuncs.getstringidentifier(
				staticMethod->GetName()),
			(int)staticMethods.size()));
	Meta::ClassBase<MetaProvider::Traits>::AddStaticMethod(staticMethod);
}

NPClassWrapper* ClassBase::GetClassWrapper()
{
	return &npClassWrapper;
}

const ClassBase::IdentifierMap& ClassBase::GetMethodsByIdentifier() const
{
	return methodsByIdentifier;
}

const ClassBase::IdentifierMap& ClassBase::GetStaticMethodsByIdentifier() const
{
	return staticMethodsByIdentifier;
}

END_INANITY_NP

// Instantiate all methods of base class.

template class Inanity::Meta::ClassBase<Inanity::Script::Np::MetaProvider::Traits>;
