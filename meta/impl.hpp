#ifndef ___INANITY_META_IMPL_HPP___
#define ___INANITY_META_IMPL_HPP___

#define META_CLASS(className, fullClassName) \
	template <> \
	Inanity::Meta::OfClass<className>::OfClass() \
		: Inanity::Meta::Class<className>(#className, #fullClassName) {} \
	template <> \
	Inanity::Meta::OfClass<className> Inanity::Meta::OfClass<className>::meta

#define META_CLASS_PARENT(parentClassName) \
	SetParent(&OfClass<parentClassName>::meta)

#define META_CLASS_CONSTRUCTOR(...) \
	template <> \
	class Inanity::Meta::OfConstructor<void (ClassType::*)(__VA_ARGS__)> \
		: public Inanity::Meta::Constructor<void (ClassType::*)(__VA_ARGS__)> \
	{ \
	public:
		OfConstructor() : Inanity::Meta::Constructor<void (ClassType::*)(__VA_ARGS__)>(#
		static OfConstructor meta;
	}; \
	template <> \
	Inanity::Meta::OfConstructor<void (ClassType::*)(__VA_ARGS__)> Inanity::Meta::OfConstructor<void (ClassType::*)(__VA_ARGS__)>::meta

	AddConstructor(&OfConstructor<void (ClassType::*)(__VA_ARGS__)>::meta)

#define META_CLASS_METHOD(methodName) \
	AddMethod(&OfMethod<decltype<&ClassType::methodName>, &ClassType::methodName>::meta)

#define META_CLASS_STATIC_METHOD(methodName) \
	AddStaticMethod(&OfStaticMethod<ClassType, decltype<&ClassType::methodName>, &ClassType::methodName>::meta)

#endif
