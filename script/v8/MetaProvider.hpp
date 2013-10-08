#ifndef ___INANITY_SCRIPT_V8_META_PROVIDER_HPP___
#define ___INANITY_SCRIPT_V8_META_PROVIDER_HPP___

#include "v8.hpp"
#include "../../meta/ClassBase.hpp"
#include "v8lib.hpp"

BEGIN_INANITY_V8

class MetaProvider
{
private:
	class CallableBase
	{
	public:
		virtual v8::FunctionCallback GetThunk() const = 0;
	};

	class NamedCallableBase : public CallableBase
	{
	private:
		const char* name;

	public:
		NamedCallableBase(const char* name)
		: name(name) {}

		const char* GetName() const
		{
			return name;
		}
	};

public:
	typedef CallableBase ConstructorBase;
	typedef NamedCallableBase FunctionBase;
	typedef NamedCallableBase MethodBase;

	template <typename ConstructorType>
	class Constructor : public ConstructorBase
	{
	public:
		v8::FunctionCallback GetThunk() const;
	};

	template <typename FunctionType, FunctionType function>
	class Function : public FunctionBase
	{
	public:
		Function(const char* name)
		: FunctionBase(name) {}

		v8::FunctionCallback GetThunk() const;
	};

	template <typename MethodType, MethodType method>
	class Method : public MethodBase
	{
	public:
		Method(const char* name)
		: MethodBase(name) {}

		v8::FunctionCallback GetThunk() const;
	};

	class ClassBase;

	struct Traits
	{
		typedef MetaProvider::ConstructorBase ConstructorBase;
		typedef MetaProvider::FunctionBase FunctionBase;
		typedef MetaProvider::MethodBase MethodBase;
		typedef MetaProvider::ClassBase ClassBase;
	};

	class ClassBase : public Meta::ClassBase<Traits>
	{
	public:
		ClassBase(const char* name, const char* fullName)
		: Meta::ClassBase<Traits>(name, fullName)
		{}

		virtual v8::FunctionCallback GetDummyConstructorThunk() const = 0;
	};

	template <typename ClassType>
	class Class : public ClassBase
	{
	public:
		Class(const char* name, const char* fullName)
		: ClassBase(name, fullName) {}
		v8::FunctionCallback GetDummyConstructorThunk() const;
	};
};

END_INANITY_V8

#endif
