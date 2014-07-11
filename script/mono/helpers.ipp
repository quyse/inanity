#include "helpers.hpp"

BEGIN_INANITY_MONO

//*** struct TypeName

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

END_INANITY_MONO
