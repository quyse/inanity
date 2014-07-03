#include "MetaProvider.ipp"

BEGIN_INANITY_MONO

const String& MetaProvider::CallableBase::GetInternalCallName() const
{
	return internalCallName;
}

void MetaProvider::ConstructorBase::SetClassName(const String& className)
{
	internalCallName = className + "::__c";
}

MetaProvider::NamedCallableBase::NamedCallableBase(const char* name)
: name(name) {}

MetaProvider::FunctionBase::FunctionBase(const char* name)
: NamedCallableBase(name) {}

void MetaProvider::FunctionBase::SetClassName(const String& className)
{
	internalCallName = className + "::__f_" + name;
}

MetaProvider::MethodBase::MethodBase(const char* name)
: NamedCallableBase(name) {}

void MetaProvider::MethodBase::SetClassName(const String& className)
{
	internalCallName = className + "::__m_" + name;
}

END_INANITY_MONO
