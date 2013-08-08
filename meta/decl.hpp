#ifndef ___INANITY_META_DECL_HPP___
#define ___INANITY_META_DECL_HPP___

// This file includes files necessary for declaring metadata.

#include "Class.hpp"

#define META_DECLARE_CLASS(className) \
	public: \
		class ClassMeta : public Inanity::Meta::Class<className> \
		{ \
		private: \
			typedef className ClassType; \
		public: \
			ClassMeta(); \
		}; \
		static ClassMeta meta

#endif
