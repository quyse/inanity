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

/// Managed pointer class.
template <typename T>
class ptr
{
private:
	/// Pointer to managed object.
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

	/// Copy constructor from another compatible pointer.
	template <typename TT>
	inline ptr(const ptr<TT>& p)
	{
		object = static_cast<TT*>(p);
		if(object) object->Reference();
#ifdef ___INANITY_TRACE_PTR
		ManagedHeapTracePtr(this, object);
#endif
	}

	/// Constructor from unmanaged pointer.
	/** Subtypes are allowed. */
	inline ptr(T* p = 0)
	{
		object = p;
		if(object) object->Reference();
#ifdef ___INANITY_TRACE_PTR
		ManagedHeapTracePtr(this, object);
#endif
	}

	inline ~ptr()
	{
		if(object) object->Dereference();
#ifdef ___INANITY_TRACE_PTR
		ManagedHeapTracePtr(this, 0);
#endif
	}

	/// Assignment operator.
	inline void operator = (const ptr<T>& p)
	{
		if(object) object->Dereference();
		object = p.object;
		if(object) object->Reference();
#ifdef ___INANITY_TRACE_PTR
		ManagedHeapTracePtr(this, object);
#endif
	}
	/// Assignment operator for compatible pointers.
	template <typename TT>
	inline void operator = (const ptr<TT>& p)
	{
		if(object) object->Dereference();
		object = static_cast<TT*>(p);
		if(object) object->Reference();
#ifdef ___INANITY_TRACE_PTR
		ManagedHeapTracePtr(this, object);
#endif
	}

	inline T& operator*() const
	{
		CHECK_OBJECT();
		return *object;
	}

	inline T* operator->() const
	{
		CHECK_OBJECT();
		return object;
	}

	/// Implicit conversion to unmanaged pointer.
	inline operator T*() const
	{
		return object;
	}

	/// Checks pointer for validity.
	inline operator bool() const
	{
		return !!object;
	}

	/// Compare operator.
	inline friend bool operator<(const ptr<T>& a, const ptr<T>& b)
	{
		return a.object < b.object;
	}

	/// Test equality operator.
	inline friend bool operator==(const ptr<T>& a, const ptr<T>& b)
	{
		return a.object == b.object;
	}

	/// Test inequality operator.
	inline friend bool operator!=(const ptr<T>& a, const ptr<T>& b)
	{
		return a.object != b.object;
	}

	/// Static cast for pointer.
	template <typename TT>
	ptr<TT> StaticCast() const
	{
		return static_cast<TT*>(object);
	}

	/// Dynamic cast for pointer.
	template <typename TT>
	ptr<TT> DynamicCast() const
	{
		return dynamic_cast<TT*>(object);
	}

	/// "Fast" cast pointer.
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

/// Explicit packing into managed pointer.
template <typename T>
inline ptr<T> MakePointer(T* p)
{
	return p;
}

#undef CHECK_OBJECT

END_INANITY

// Support for pointer's hash calculation.
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
