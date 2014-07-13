#ifndef ___INANITY_SCRIPT_MONO_VALUES_IPP___
#define ___INANITY_SCRIPT_MONO_VALUES_IPP___

#include "values.hpp"
#include "Any.hpp"
#include "State.hpp"
#include "../../Exception.hpp"
#include <mono/metadata/object.h>
#include <mono/metadata/appdomain.h>

BEGIN_INANITY_MONO

template <>
struct Value<bool>
{
	typedef bool ValueType;
	typedef MonoBoolean MonoType;

	static inline bool From(MonoBoolean value)
	{
		return !!value;
	}
	static inline MonoBoolean To(bool value)
	{
		return (MonoBoolean)value;
	}
};

#define IDENTITY(type) \
	template <> \
	struct Value<type> \
	{ \
		typedef type ValueType; \
		typedef type MonoType; \
		static inline type From(type value) { return value; } \
		static inline type To(type value) { return value; } \
	}

IDENTITY(int);
IDENTITY(unsigned int);
IDENTITY(long);
IDENTITY(unsigned long);
IDENTITY(long long);
IDENTITY(unsigned long long);

#undef IDENTITY

#define BOXED(type, monotype) \
	template <> \
	struct Value<type> \
	{ \
		typedef type ValueType; \
		typedef MonoObject* MonoType; \
		static inline type From(MonoObject* object) \
		{ \
			return *(type*)mono_object_unbox(object); \
		} \
	}

BOXED(float, single);
BOXED(double, double);

#undef BOXED

template <>
struct Value<String>
{
	typedef String ValueType;
	typedef MonoString* MonoType;

	static inline String From(MonoString* value)
	{
		char* str = mono_string_to_utf8(value);
		String r(str);
		mono_free(str);
		return r;
	}
	static inline MonoString* To(const String& value)
	{
		return mono_string_new_len(State::instance->GetDomain(), value.c_str(), value.length());
	}
};

template <>
struct Value<const char*>
{
	typedef String ValueType;
	typedef MonoString* MonoType;

	static inline String From(MonoString* value)
	{
		return Value<String>::From(value);
	}
	static inline MonoString* To(const char* value)
	{
		return mono_string_new(State::instance->GetDomain(), value);
	}
};

template <>
struct Value<const String&> : public Value<String> {};

template <>
struct Value<ptr<Script::Any> >
{
	typedef ptr<Script::Any> ValueType;
	typedef MonoObject* MonoType;

	static inline ptr<Script::Any> From(MonoObject* value)
	{
		return State::instance->CreateAny(value);
	}
	static inline MonoObject* To(ptr<Script::Any> value)
	{
		return value.FastCast<Any>()->GetObject();
	}
};

template <typename ObjectType>
struct Value<ptr<ObjectType> >
{
	typedef ptr<ObjectType> ValueType;
	typedef MonoObject* MonoType;

	static inline ptr<ObjectType> From(MonoObject* value)
	{
		return static_cast<ObjectType*>(State::instance->UnwrapObject(value));
	}
	static inline MonoObject* To(ptr<ObjectType> value)
	{
		return State::instance->ConvertObject(Meta::MetaOf<MetaProvider, ObjectType>(), &*value);
	}
};

template <typename T>
struct Value
{
	typedef T ValueType;
	typedef MonoObject* MonoType;

	static inline T From(MonoObject* value)
	{
		return ConvertFromScript<MetaProvider, T>(State::instance->CreateAny(value));
	}
	static inline MonoObject* To(const T& value)
	{
		return ConvertToScript<MetaProvider, T>(State::instance, value).template FastCast<Any>().GetObject();
	}
};

template <typename T>
struct Value<const T&> : public Value<T> {};

END_INANITY_MONO

#endif
