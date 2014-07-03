#ifndef ___INANITY_SCRIPT_MONO_META_PROVIDER_HPP___
#define ___INANITY_SCRIPT_MONO_META_PROVIDER_HPP___

#include "mono.hpp"
#include "../../String.hpp"

BEGIN_INANITY_MONO

class ClassBase;

class MetaProvider
{
public:
	class CallableBase
	{
	protected:
		String internalCallName;

	public:
		const String& GetInternalCallName() const;
		virtual void RegisterThunk() = 0;
	};

	class ConstructorBase : public CallableBase
	{
	public:
		void SetClassName(const String& className);
	};

	class NamedCallableBase : public CallableBase
	{
	protected:
		const char* name;

	public:
		NamedCallableBase(const char* name);

		const char* GetName() const;
	};

	class FunctionBase : public NamedCallableBase
	{
	public:
		FunctionBase(const char* name);
		void SetClassName(const String& className);
	};

	class MethodBase : public NamedCallableBase
	{
	public:
		MethodBase(const char* name);
		void SetClassName(const String& className);
	};

	template <typename ClassType, typename... Args>
	class Constructor;

	template <typename FunctionType, FunctionType function>
	class Function;

	template <typename MethodType, MethodType method>
	class Method;

	typedef Mono::ClassBase ClassBase;

	struct Traits
	{
		typedef MetaProvider::ConstructorBase ConstructorBase;
		typedef MetaProvider::FunctionBase FunctionBase;
		typedef MetaProvider::MethodBase MethodBase;
		typedef MetaProvider::ClassBase ClassBase;
	};

	template <typename ClassType>
	class Class;
};

END_INANITY_MONO

#endif
