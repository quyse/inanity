#include "MetaProvider.hpp"
#include "State.hpp"
#include "thunks.hpp"
#include "ClassBase.hpp"
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
