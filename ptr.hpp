#ifndef ___INANITY_POINTER_HPP___
#define ___INANITY_POINTER_HPP___

#include "config.hpp"
#include <functional>

BEGIN_INANITY

#if defined(_DEBUG) && defined(_M_IX86)
#define CHECK_OBJECT() \
	if(!object) \
		__asm int 3;
#else
#define CHECK_OBJECT()
#endif

#ifdef ___INANITY_TRACE_PTR
// defined in ManagedHeap.cpp
void ManagedHeapTracePtr(void* ptr, void* object);
#endif

/// Pointer to managed object.
template <typename T>
class ptr
{
private:
	/// The pointer.
	T* object;

public:
	/// Copy constructor.
	inline ptr(const ptr<T>& p)
	{
		object = p.object;
		if(object) object->Reference();
#ifdef ___INANITY_TRACE_PTR
		ManagedHeapTracePtr(this, object);
#endif
	}

	/// Templated copy constructor.
	template <typename TT>
	inline ptr(const ptr<TT>& p)
	{
		object = static_cast<TT*>(p);
		if(object) object->Reference();
#ifdef ___INANITY_TRACE_PTR
		ManagedHeapTracePtr(this, object);
#endif
	}

	/// Plain pointer constructor.
	/// Derived types are served automatically.
	inline ptr(T* p = 0)
	{
		object = p;
		if(object) object->Reference();
#ifdef ___INANITY_TRACE_PTR
		if(object) ManagedHeapTracePtr(this, object);
#endif
	}

	inline ~ptr()
	{
		if(object) object->Dereference();
#ifdef ___INANITY_TRACE_PTR
		ManagedHeapTracePtr(this, 0);
#endif
	}

	/* Assignment operators should reference new
	object before, dereference old object after.
	In case of self-assigning (which is considered rare),
	object would not disappear with heap corruption.
	*/

	/// Assign operator.
	inline void operator = (const ptr<T>& p)
	{
		T* previousObject = object;
		object = p.object;
		if(object) object->Reference();
		if(previousObject) previousObject->Dereference();
#ifdef ___INANITY_TRACE_PTR
		ManagedHeapTracePtr(this, object);
#endif
	}
	/// Templated assign operator.
	template <typename TT>
	inline void operator = (const ptr<TT>& p)
	{
		T* previousObject = object;
		object = static_cast<TT*>(p);
		if(object) object->Reference();
		if(previousObject) previousObject->Dereference();
#ifdef ___INANITY_TRACE_PTR
		ManagedHeapTracePtr(this, object);
#endif
	}

	/// Dereference pointer operator.
	inline T& operator*() const
	{
		CHECK_OBJECT();
		return *object;
	}

	/// Method call operator.
	inline T* operator->() const
	{
		CHECK_OBJECT();
		return object;
	}

	/// Implicit conversion to plain pointer.
	inline operator T*() const
	{
		return object;
	}

	/// Validity check operator.
	inline operator bool() const
	{
		return !!object;
	}

	/// Compare operator.
	inline friend bool operator<(const ptr<T>& a, const ptr<T>& b)
	{
		return a.object < b.object;
	}

	/// Equality operator.
	inline friend bool operator==(const ptr<T>& a, const ptr<T>& b)
	{
		return a.object == b.object;
	}

	/// Inequality operator.
	inline friend bool operator!=(const ptr<T>& a, const ptr<T>& b)
	{
		return a.object != b.object;
	}

	/// Static cast method.
	template <typename TT>
	ptr<TT> StaticCast() const
	{
		return static_cast<TT*>(object);
	}

	/// Dynamic cast method.
	template <typename TT>
	ptr<TT> DynamicCast() const
	{
		return dynamic_cast<TT*>(object);
	}

	/// "Fast cast" method.
	template <typename TT>
	ptr<TT> FastCast() const
	{
#ifdef _DEBUG
		return dynamic_cast<TT*>(object);
#else
		return static_cast<TT*>(object);
#endif
	}
};

/// Explicit wrapping plain pointer into managed one.
template <typename T>
inline ptr<T> MakePointer(T* p)
{
	return p;
}

#undef CHECK_OBJECT

END_INANITY

// Hash operator for managed pointers.
namespace std
{
	template <typename T>
	struct hash<Inanity::ptr<T> >
	{
		size_t operator()(const Inanity::ptr<T>& p) const
		{
			return reinterpret_cast<size_t>(static_cast<T*>(p));
		}
	};
}

#endif
