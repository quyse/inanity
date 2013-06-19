#ifndef ___INANITY_META_IMPL_HPP___
#define ___INANITY_META_IMPL_HPP___

#include "Class.ipp"
#include "Constructor.ipp"
#include "Method.ipp"
#include "Function.ipp"
#include "Extension.ipp"

#define META_CLASS(className, fullClassName) \
	className::ClassMeta className::meta; \
	className::ClassMeta::ClassMeta() \
		: Inanity::Meta::Class<className>(#className, #fullClassName) {
#define META_CLASS_END() }

#define META_CLASS_PARENT(parentClassName) \
	SetParent(&parentClassName::meta)

#define META_CONSTRUCTOR(...) \
	{ \
		static Inanity::Meta::Constructor<void (ClassType::*)(__VA_ARGS__)> c; \
		SetConstructor(&c); \
	}

#define META_METHOD(methodName) \
	{ \
		static Inanity::Meta::Method<decltype(&ClassType::methodName), &ClassType::methodName> m(#methodName); \
		AddMethod(&m); \
	}

#define META_STATIC_METHOD(methodName) \
	{ \
		static Inanity::Meta::Function<decltype(&ClassType::methodName), &ClassType::methodName> f(#methodName); \
		AddStaticMethod(&f); \
	}

#endif
