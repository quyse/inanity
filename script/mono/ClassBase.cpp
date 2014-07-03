#include "ClassBase.hpp"
#include "State.hpp"
#include "../../meta/ClassBase.ipp"
#include "../../Exception.hpp"
#include <mono/metadata/class.h>

BEGIN_INANITY_MONO

ClassBase::ClassBase(const char* name, const char* fullName) :
	Parent(name, fullName),
	monoClass(nullptr),
	monoFullName(fullName)
{
	// split full name into namespace and name
	for(int i = (int)monoFullName.length() - 1; i >= 0; --i)
		if(monoFullName[i] == '.')
		{
			monoClassName = monoFullName.substr(i + 1);
			monoNamespace = monoFullName.substr(0, i);
			break;
		}
}

void ClassBase::SetConstructor(ConstructorBase* constructor)
{
	constructor->SetClassName(monoFullName);
	Parent::SetConstructor(constructor);
}

void ClassBase::AddMethod(MethodBase* method)
{
	method->SetClassName(monoFullName);
	Parent::AddMethod(method);
}

void ClassBase::AddStaticMethod(FunctionBase* staticMethod)
{
	staticMethod->SetClassName(monoFullName);
	Parent::AddStaticMethod(staticMethod);
}

MonoClass* ClassBase::GetMonoClass() const
{
	if(!monoClass)
	{
		monoClass = mono_class_from_name(State::instance->GetBaseImage(), monoNamespace.c_str(), monoClassName.c_str());
		if(!monoClass)
			THROW(String("Can't get Mono class for ") + fullName);
	}
	return monoClass;
}

END_INANITY_MONO
