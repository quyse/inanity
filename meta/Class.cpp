#include "Class.ipp"

BEGIN_INANITY_META

ClassBase::ClassBase(const char* name, const char* fullName, ClassBase* parent)
: name(name), fullName(fullName), parent(parent) {}

const char* ClassBase::GetName() const
{
	return name;
}

const char* ClassBase::GetFullName() const
{
	return fullName;
}

ClassBase* ClassBase::GetParent() const
{
	return parent;
}

void ClassBase::AddConstructor(ConstructorBase* constructor)
{
	constructors.push_back(constructor);
}

void ClassBase::AddMethod(MethodBase* method)
{
	methods.push_back(method);
}

void ClassBase::AddStaticMethod(FunctionBase* staticMethod)
{
	staticMethods.push_back(staticMethod);
}

const ClassBase::Constructors& ClassBase::GetConstructors() const
{
	return constructors;
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
