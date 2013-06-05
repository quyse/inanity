#ifndef ___INANITY_META_DECL_HPP___
#define ___INANITY_META_DECL_HPP___

// This file includes files necessary for declaring metadata.

#include "of.hpp"

#define META_DECLARE_CLASS(className) \
	template <> \
	class Inanity::Meta::OfClass<className> : public Inanity::Meta::Class<className> \
	{ \
	public: \
		OfClass(); \
		static OfClass meta;
	}

#endif
