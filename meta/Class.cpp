#include "Class.ipp"

BEGIN_INANITY_META

ClassBase::ClassBase(const char* name, const char* fullName)
: name(name), fullName(fullName) {}

const char* ClassBase::GetName() const
{
	return name;
}

const char* ClassBase::GetFullName() const
{
	return fullName;
}

void ClassBase::SetParent(ClassBase* parent)
{
	this->parent = parent;
}

ClassBase* ClassBase::GetParent() const
{
	return parent;
}

void ClassBase::SetConstructor(ConstructorBase* constructor)
{
	this->constructor = constructor;
}

void ClassBase::AddMethod(MethodBase* method)
{
	methods.push_back(method);
}

void ClassBase::AddStaticMethod(FunctionBase* staticMethod)
{
	staticMethods.push_back(staticMethod);
}

ConstructorBase* ClassBase::GetConstructor() const
{
	return constructor;
}

const ClassBase::Methods& ClassBase::GetMethods() const
{
	return methods;
}

const ClassBase::StaticMethods& ClassBase::GetStaticMethods() const
{
	return staticMethods;
}

END_INANITY_META
