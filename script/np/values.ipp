#ifndef ___INANITY_SCRIPT_NP_VALUES_IPP___
#define ___INANITY_SCRIPT_NP_VALUES_IPP___

#include "values.hpp"
#include "Any.hpp"
#include "State.hpp"
#include "MetaProvider.hpp"
#include "ClassBase.hpp"
#include "wrappers.hpp"
#include "../../platform/NpapiPlugin.hpp"
#include "../../Exception.hpp"
#include <cstring>

BEGIN_INANITY_NP

/*
NPVariants sent as argument to 'From' methods
are not referenced/dereferenced.
Returned NPVariant from 'To' methods should be
released with NPN_ReleaseVariantValue.
*/

template <>
struct Value<bool>
{
	typedef bool ValueType;

	static inline bool From(NPVariant value)
	{
		if(NPVARIANT_IS_VOID(value) || NPVARIANT_IS_NULL(value))
			return false;
		if(NPVARIANT_IS_BOOLEAN(value))
			return NPVARIANT_TO_BOOLEAN(value);
		if(NPVARIANT_IS_INT32(value))
			return !!NPVARIANT_TO_INT32(value);
		if(NPVARIANT_IS_DOUBLE(value))
			return !!NPVARIANT_TO_DOUBLE(value);
		if(NPVARIANT_IS_STRING(value))
			return !!NPVARIANT_TO_STRING(value).UTF8Length;
		if(NPVARIANT_IS_OBJECT(value))
			return !!NPVARIANT_TO_OBJECT(value);
		return false;
	}

	static inline NPVariant To(bool value)
	{
		NPVariant variant;
		BOOLEAN_TO_NPVARIANT(value, variant);
		return variant;
	}
};

template <>
struct Value<int>
{
	typedef int ValueType;

	static inline int From(NPVariant value)
	{
		if(NPVARIANT_IS_BOOLEAN(value))
			return (int)NPVARIANT_TO_BOOLEAN(value);
		if(NPVARIANT_IS_INT32(value))
			return (int)NPVARIANT_TO_INT32(value);
		if(NPVARIANT_IS_DOUBLE(value))
			return (int)NPVARIANT_TO_DOUBLE(value);
		return 0;
	}

	static inline NPVariant To(int value)
	{
		NPVariant variant;
		INT32_TO_NPVARIANT(value, variant);
		return variant;
	}
};

template <>
struct Value<long>
{
	typedef long ValueType;

	static inline long From(NPVariant value)
	{
		if(NPVARIANT_IS_BOOLEAN(value))
			return (long)NPVARIANT_TO_BOOLEAN(value);
		if(NPVARIANT_IS_INT32(value))
			return (long)NPVARIANT_TO_INT32(value);
		if(NPVARIANT_IS_DOUBLE(value))
			return (long)NPVARIANT_TO_DOUBLE(value);
		return 0;
	}

	static inline NPVariant To(long value)
	{
		NPVariant variant;
		INT32_TO_NPVARIANT(value, variant);
		return variant;
	}
};

template <>
struct Value<double>
{
	typedef double ValueType;

	static inline double From(NPVariant value)
	{
		if(NPVARIANT_IS_BOOLEAN(value))
			return (double)NPVARIANT_TO_BOOLEAN(value);
		if(NPVARIANT_IS_INT32(value))
			return (double)NPVARIANT_TO_INT32(value);
		if(NPVARIANT_IS_DOUBLE(value))
			return (double)NPVARIANT_TO_DOUBLE(value);
		return 0;
	}

	static inline NPVariant To(double value)
	{
		NPVariant variant;
		DOUBLE_TO_NPVARIANT(value, variant);
		return variant;
	}
};

template <>
struct Value<float>
{
	typedef float ValueType;

	static inline float From(NPVariant value)
	{
		if(NPVARIANT_IS_BOOLEAN(value))
			return (float)NPVARIANT_TO_BOOLEAN(value);
		if(NPVARIANT_IS_INT32(value))
			return (float)NPVARIANT_TO_INT32(value);
		if(NPVARIANT_IS_DOUBLE(value))
			return (float)NPVARIANT_TO_DOUBLE(value);
		return 0;
	}

	static inline NPVariant To(float value)
	{
		NPVariant variant;
		DOUBLE_TO_NPVARIANT((double)value, variant);
		return variant;
	}
};

template <>
struct Value<unsigned int>
{
	typedef unsigned int ValueType;

	static inline unsigned int From(NPVariant value)
	{
		return (unsigned int)Value<double>::From(value);
	}

	static inline NPVariant To(unsigned int value)
	{
		return Value<double>::To((double)value);
	}
};

template <>
struct Value<unsigned long>
{
	typedef unsigned long ValueType;

	static inline unsigned long From(NPVariant value)
	{
		return (unsigned long)Value<double>::From(value);
	}

	static inline NPVariant To(unsigned long value)
	{
		return Value<double>::To((double)value);
	}
};

template <>
struct Value<long long>
{
	typedef long long ValueType;

	static inline long long From(NPVariant value)
	{
		return (long long)Value<double>::From(value);
	}

	static inline NPVariant To(long long value)
	{
		return Value<double>::To((double)value);
	}
};

template <>
struct Value<unsigned long long>
{
	typedef unsigned long long ValueType;

	static inline unsigned long long From(NPVariant value)
	{
		return (unsigned long long)Value<double>::From(value);
	}

	static inline NPVariant To(unsigned long long value)
	{
		return Value<double>::To((double)value);
	}
};

template <>
struct Value<String>
{
	typedef String ValueType;

	static inline String From(NPVariant value)
	{
		if(NPVARIANT_IS_STRING(value))
		{
			const NPString& str = NPVARIANT_TO_STRING(value);
			return String(str.UTF8Characters, str.UTF8Length);
		}
		return String();
	}

	static inline NPVariant To(const String& value)
	{
		char* mem = (char*)Platform::NpapiPlugin::browserFuncs.memalloc(value.length() + 1);
		memcpy(mem, value.c_str(), value.length() + 1);
		NPVariant variant;
		STRINGN_TO_NPVARIANT(mem, value.length(), variant);
		return variant;
	}
};

template <>
struct Value<const char*>
{
	typedef String ValueType;

	static inline String From(NPVariant value)
	{
		return Value<String>::From(value);
	}

	static inline NPVariant To(const char* value)
	{
		int length = strlen(value);
		char* mem = (char*)Platform::NpapiPlugin::browserFuncs.memalloc(length + 1);
		memcpy(mem, value, length + 1);
		NPVariant variant;
		STRINGN_TO_NPVARIANT(mem, length, variant);
		return variant;
	}
};

template <>
struct Value<const String&>
{
	typedef String ValueType;

	static inline String From(NPVariant value)
	{
		return Value<String>::From(value);
	}
};

template <>
struct Value<ptr<Script::Any> >
{
	typedef ptr<Script::Any> ValueType;

	static inline ptr<Script::Any> From(NPVariant value)
	{
		return State::GetCurrent()->CreateAnyViaCopy(value);
	}

	static inline NPVariant To(ptr<Script::Any> value)
	{
		NPVariant variant = fast_cast<Any*>(&*value)->GetVariant();
		State::DuplicateVariant(variant);
		return variant;
	}
};

template <typename ObjectType>
struct Value<ptr<ObjectType> >
{
	typedef ptr<ObjectType> ValueType;

	static inline ptr<ObjectType> From(NPVariant value)
	{
		// if it's null
		if(NPVARIANT_IS_NULL(value))
			return nullptr;

		// get target class
		MetaProvider::ClassBase* targetClass = Meta::MetaOf<MetaProvider, ObjectType>();

		// it should be an object
		if(!NPVARIANT_IS_OBJECT(value))
			THROW(String("Can't get ") + targetClass->GetFullName() + " from non-object");

		NPObject* npObject = NPVARIANT_TO_OBJECT(value);

		// object should be of registered class
		State* state = State::GetCurrent();
		if(!state->CheckClass(npObject->_class))
			THROW(String("Can't get ") + targetClass->GetFullName() + " from object of non-Inanity class");

		// get object wrapper
		NPObjectWrapper* objectWrapper = static_cast<NPObjectWrapper*>(npObject);

		// check if object was reclaimed
		if(!objectWrapper->object)
			THROW("Object was reclaimed");

		// get class meta
		MetaProvider::ClassBase* classMeta = objectWrapper->GetClassWrapper()->classMeta;

		// check that object can cast to requested type
		if(!classMeta->CanCastTo(Meta::MetaOf<MetaProvider, ObjectType>()))
			THROW(String("Can't cast ") + classMeta->GetFullName() + " to " + targetClass->GetFullName());

		// do cast and return
		return objectWrapper->object.FastCast<ObjectType>();
	}

	static inline NPVariant To(ptr<ObjectType> value)
	{
		if(value)
			return State::GetCurrent()->ConvertObject(
				Meta::MetaOf<MetaProvider, ObjectType>(),
				static_cast<RefCounted*>(&*value));
		else
		{
			NPVariant variant;
			NULL_TO_NPVARIANT(variant);
			return variant;
		}
	}
};

template <typename T>
struct Value
{
	typedef T ValueType;

	static inline T From(NPVariant value)
	{
		return ConvertFromScript<T>(State::GetCurrent()->CreateAnyViaCopy(value));
	}

	static inline NPVariant To(const T& value)
	{
		ptr<Any> any = ConvertToScript<T>(State::GetCurrent(), value).template FastCast<Any>();
		NPVariant variant = any->GetVariant();
		State::DuplicateVariant(variant);
		return variant;
	}
};

template <typename T>
struct Value<const T&>
{
	typedef T ValueType;

	static inline T From(NPVariant value)
	{
		return ConvertFromScript<T>(State::GetCurrent()->CreateAnyViaCopy(value));
	}

	static inline NPVariant To(const T& value)
	{
		ptr<Any> any = ConvertToScript<T>(State::GetCurrent(), value).template FastCast<Any>();
		NPVariant variant = any->GetVariant();
		State::DuplicateVariant(variant);
		return variant;
	}
};

END_INANITY_NP

#endif
