#include "MetaProvider.hpp"
#include "State.hpp"
#include "thunks.hpp"
#include "values.hpp"
#include "ClassBase.hpp"
#include "BaseAssemblyGenerator.hpp"
#include <mono/metadata/object.h>

BEGIN_INANITY_MONO

//*** helper struct TypeName

template <typename T>
struct TypeName;
template <> struct TypeName<void>
{
	static const char* GetName()
	{
		return "void";
	}
};
template <> struct TypeName<bool>
{
	static const char* GetName()
	{
		return "bool";
	}
};
template <> struct TypeName<int32_t>
{
	static const char* GetName()
	{
		return "int32";
	}
};
template <> struct TypeName<uint32_t>
{
	static const char* GetName()
	{
		return "uint32";
	}
};
template <> struct TypeName<int64_t>
{
	static const char* GetName()
	{
		return "int64";
	}
};
template <> struct TypeName<uint64_t>
{
	static const char* GetName()
	{
		return "uint64";
	}
};
template <> struct TypeName<float>
{
	static const char* GetName()
	{
		return "float";
	}
};
template <> struct TypeName<double>
{
	static const char* GetName()
	{
		return "double";
	}
};
template <> struct TypeName<String>
{
	static const char* GetName()
	{
		return "string";
	}
};
template <typename ObjectType>
struct TypeName<ptr<ObjectType> >
{
	static const char* GetName()
	{
		return Meta::MetaOf<MetaProvider, ObjectType>()->GetFullName();
	}
};

//*** struct TypeNameHelper

template <typename Callable>
struct TypeNameHelper;
// for functions
template <typename ReturnType, typename... Args>
struct TypeNameHelper<ReturnType (*)(Args...)>
{
	template <typename Callback>
	static void GetNames(Callback callback)
	{
		const char* argumentTypes[] = { nullptr, TypeName<typename Value<Args>::ValueType>::GetName()... };
		callback(TypeName<ReturnType>::GetName(), sizeof...(Args), argumentTypes + 1);
	}
};
// for non-const methods
template <typename ReturnType, typename ClassType, typename... Args>
struct TypeNameHelper<ReturnType (ClassType::*)(Args...)>
: public TypeNameHelper<ReturnType (*)(Args...)> {};
// for const methods
template <typename ReturnType, typename ClassType, typename... Args>
struct TypeNameHelper<ReturnType (ClassType::*)(Args...) const>
: public TypeNameHelper<ReturnType (*)(Args...)> {};

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
