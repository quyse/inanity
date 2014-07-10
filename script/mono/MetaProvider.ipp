#include "MetaProvider.hpp"
#include "State.hpp"
#include "thunks.hpp"
#include "values.hpp"
#include "helpers.hpp"
#include "ClassBase.hpp"
#include "BaseAssemblyGenerator.hpp"
#include <mono/metadata/object.h>

BEGIN_INANITY_MONO

//*** class MetaProvider::Constructor

template <typename ClassType, typename... Args>
class MetaProvider::Constructor : public MetaProvider::ConstructorBase
{
public:
	void RegisterThunk()
	{
		mono_add_internal_call(GetInternalCallName().c_str(), (const void*)&ConstructorHelper<ClassType, Args...>::Thunk);
	}

	void RegisterForGeneration(BaseAssemblyGenerator* generator)
	{
		const char* argumentTypes[] = { nullptr, TypeName<Args>::GetName()... };
		generator->RegisterConstructor(sizeof...(Args), argumentTypes + 1);
	}
};

//*** class MetaProvider::Function

template <typename FunctionType, FunctionType function>
class MetaProvider::Function : public MetaProvider::FunctionBase
{
public:
	Function(const char* name) : FunctionBase(name) {}

	void RegisterThunk()
	{
		mono_add_internal_call(GetInternalCallName().c_str(), (const void*)&FunctionHelper<FunctionType, function>::Thunk);
	}

	void RegisterForGeneration(BaseAssemblyGenerator* generator)
	{
		TypeNameHelper<FunctionType>::GetNames([this, generator](const char* returnType, int argumentsCount, const char** argumentTypes)
		{
			generator->RegisterFunction(name, returnType, argumentsCount, argumentTypes);
		});
	}
};

//*** class MetaProvider::Method

template <typename MethodType, MethodType method>
class MetaProvider::Method : public MetaProvider::MethodBase
{
public:
	Method(const char* name) : MethodBase(name) {}

	void RegisterThunk()
	{
		mono_add_internal_call(GetInternalCallName().c_str(), (const void*)&MethodHelper<MethodType, method>::Thunk);
	}

	void RegisterForGeneration(BaseAssemblyGenerator* generator)
	{
		TypeNameHelper<MethodType>::GetNames([this, generator](const char* returnType, int argumentsCount, const char** argumentTypes)
		{
			generator->RegisterMethod(name, returnType, argumentsCount, argumentTypes);
		});
	}
};

//*** class MetaProvider::Class

template <typename ClassType>
class MetaProvider::Class : public MetaProvider::ClassBase
{
public:
	Class(const char* name, const char* fullName)
	: ClassBase(name, fullName) {}
};

END_INANITY_MONO
