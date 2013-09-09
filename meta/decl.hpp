#ifndef ___INANITY_META_DECL_HPP___
#define ___INANITY_META_DECL_HPP___

#include "meta.hpp"

// This file includes files necessary for declaring metadata.

BEGIN_INANITY_META

class ClassBase;

END_INANITY_META;

#define META_DECLARE_CLASS(className) \
	public: \
		static Inanity::Meta::ClassBase* GetMeta(); \
	private: \
		class ClassMeta; \
		static ClassMeta meta

#endif
