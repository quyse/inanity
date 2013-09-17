#include "values.hpp"
#include "../../String.hpp"
#include "../../Exception.hpp"

BEGIN_INANITY_V8

template <>
struct Value<bool>
{
	typedef bool ValueType;

	static inline bool From(v8::Local<v8::Value> value)
	{
		return value->BooleanValue();
	}

	static inline v8::Local<v8::Value> To(bool value)
	{
		return v8::BooleanObject::New(value);
	}
};

template <>
struct Value<int>
{
	typedef int ValueType;

	static inline int From(v8::Local<v8::Value> value)
	{
		return value->Int32Value();
	}

	static inline v8::Local<v8::Value> To(int value)
	{
		return v8::NumberObject::New((double)value);
	}
};

template <>
struct Value<double>
{
	typedef double ValueType;

	static inline double From(v8::Local<v8::Value> value)
	{
		return value->NumberValue();
	}

	static inline v8::Local<v8::Value> To(double value)
	{
		return v8::NumberObject::New(value);
	}
};

template <>
struct Value<float>
{
	typedef float ValueType;

	static inline float From(v8::Local<v8::Value> value)
	{
		return (float)value->NumberValue();
	}

	static inline v8::Local<v8::Value> To(float value)
	{
		return v8::NumberObject::New((double)value);
	}
};

template <>
struct Value<unsigned int>
{
	typedef unsigned int ValueType;

	static inline unsigned int From(v8::Local<v8::Value> value)
	{
		return value->Uint32Value();
	}

	static inline v8::Local<v8::Value> To(unsigned int value)
	{
		return v8::NumberObject::New((double)value);
	}
};

template <>
struct Value<long long>
{
	typedef long long ValueType;

	static inline long long From(v8::Local<v8::Value> value)
	{
		return value->IntegerValue();
	}

	static inline v8::Local<v8::Value> To(long long value)
	{
		return v8::NumberObject::New((double)value);
	}
};

template <>
struct Value<unsigned long long>
{
	typedef unsigned long long ValueType;

	static inline unsigned long long From(v8::Local<v8::Value> value)
	{
		return value->IntegerValue();
	}

	static inline v8::Local<v8::Value> To(unsigned long long value)
	{
		return v8::NumberObject::New((double)value);
	}
};

template <>
struct Value<const char*>
{
	typedef String ValueType;

	static inline String From(v8::Local<v8::Value> value)
	{
		v8::String::Utf8Value s(value);
		return String(*s, s.length());
	}

	static inline v8::Local<v8::Value> To(const char* value)
	{
		return v8::String::New(value);
	}
};

template <>
struct Value<String>
{
	typedef String ValueType;

	static inline String From(v8::Local<v8::Value> value)
	{
		v8::String::Utf8Value s(value);
		return String(*s, s.length());
	}

	static inline v8::Local<v8::Value> To(const String& value)
	{
		return v8::String::New(value.c_str(), (int)value.length());
	}
};

template <>
struct Value<const String&>
{
	typedef String ValueType;

	static inline String From(v8::Local<v8::Value> value)
	{
		v8::String::Utf8Value s(value);
		return String(*s, s.length());
	}
};

/*
ptr<Object> is represented as an External, or as null (zero pointer).
External of corrent object contains non-null pointer.
External with null pointer is a wiped object, i.e. object which was
reclamed from script by C++ code.
Other values are not accepted (i.e. zero, undefined).
*/
template <typename ObjectType>
struct Value<ptr<ObjectType> >
{
	typedef ptr<ObjectType> ValueType;

	static inline ptr<ObjectType> From(v8::Local<v8::Value> value)
	{
		// get internal field of an object
		v8::Local<v8::Value> thisValue = value->ToObject()->GetInternalField(0);

		// if it's null object
		if(thisValue->IsNull())
			return 0;

		// otherwise it should be an external
		if(!thisValue->IsExternal())
		{
			v8::String::Utf8Value s(thisValue);
			THROW(ObjectType::GetMeta()->GetFullName() + String(" instance can't be obtained from ") + *s);
		}

		// get the value
		void* externalValue = v8::External::Cast(*thisValue)->Value();

		// if the value is null, the object was reclaimed
		if(!externalValue)
			THROW(ObjectType::GetMeta()->GetFullName() + String(" instance was reclaimed"));

		ObjectType* object = fast_cast<ObjectType*>((Object*)externalValue);

		return object;
	}

	static inline v8::Local<v8::Value> To(ptr<ObjectType> value)
	{
		if(value)
			return State::GetCurrent()->ConvertObject(ObjectType::GetMeta(), static_cast<Object*>(&*value));
		else
			return v8::Null();
	}
};

END_INANITY_V8
