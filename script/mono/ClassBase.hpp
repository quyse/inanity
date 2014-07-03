#ifndef ___INANITY_SCRIPT_MONO_CLASS_BASE_HPP___
#define ___INANITY_SCRIPT_MONO_CLASS_BASE_HPP___

#include "mono.hpp"
#include "MetaProvider.hpp"
#include "../../meta/ClassBase.hpp"
#include "../../String.hpp"
#include <mono/metadata/metadata.h>

BEGIN_INANITY_MONO

class ClassBase : public Meta::ClassBase<MetaProvider::Traits>
{
private:
	typedef Meta::ClassBase<MetaProvider::Traits> Parent;

	mutable MonoClass* monoClass;
	String monoFullName;
	String monoNamespace;
	String monoClassName;

public:
	ClassBase(const char* name, const char* fullName);

	void SetConstructor(ConstructorBase* constructor);
	void AddMethod(MethodBase* method);
	void AddStaticMethod(FunctionBase* staticMethod);

	MonoClass* GetMonoClass() const;
};

END_INANITY_MONO

#endif
