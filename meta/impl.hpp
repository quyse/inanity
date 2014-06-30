#ifndef ___INANITY_META_IMPL_HPP___
#define ___INANITY_META_IMPL_HPP___

#include "ClassBase.ipp"

// Before inclusion of this header, do the following:
// * include header of needed meta provider class
// * define a macro META_PROVIDER to a full name of the meta provider class
// After inclusion you likely want to undef META_PROVIDER.

#ifndef META_PROVIDER
#error "META_PROVIDER isn't defined"
#endif

#define META_CLASS(className, fullClassName) \
	BEGIN_INANITY_META \
	template <> \
	class Class<META_PROVIDER, className> : public META_PROVIDER::Class<className> \
	{ \
	private: \
		typedef META_PROVIDER Provider; \
		typedef className ClassType; \
		template <typename... Args> \
		/* HelperSetConstructor class is needed to workaround empty __VA_ARGS__ problem. */ \
		struct HelperSetConstructor \
		{ \
			typedef Provider::Constructor<ClassType, Args...> Type; \
		}; \
	public: \
		Class(); \
	}; \
	template <> \
	META_PROVIDER::ClassBase* MetaOf<META_PROVIDER, className>() \
	{ \
		static Class<META_PROVIDER, className> instance; \
		return &instance; \
	} \
	Class<META_PROVIDER, className>::Class() \
		: META_PROVIDER::Class<className>(#className, #fullClassName) \
	{
#define META_CLASS_END() } END_INANITY_META

#define META_CLASS_PARENT(parentClassName) \
	SetParent(MetaOf<Provider, parentClassName>())

#define META_CONSTRUCTOR(...) \
	{ \
		static typename HelperSetConstructor<__VA_ARGS__>::Type c; \
		SetConstructor(&c); \
	}

#define META_METHOD(methodName) \
	{ \
		static Provider::Method<decltype(&ClassType::methodName), &ClassType::methodName> m(#methodName); \
		AddMethod(&m); \
	}

#define META_METHOD_FULL(methodName, methodType, methodScriptAlias) \
	{ \
		static Provider::Method<methodType, &ClassType::methodName> m(#methodScriptAlias); \
		AddMethod(&m); \
	}

#define META_STATIC_METHOD(methodName) \
	{ \
		static Provider::Function<decltype(&ClassType::methodName), &ClassType::methodName> f(#methodName); \
		AddStaticMethod(&f); \
	}

#define META_STATIC_METHOD_FULL(methodName, methodType, methodScriptAlias) \
	{ \
		static Provider::Function<methodType, &ClassType::methodName> f(#methodScriptAlias); \
		AddStaticMethod(&f); \
	}

#endif
