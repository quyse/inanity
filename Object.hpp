#ifndef ___INANITY_OBJECT_HPP___
#define ___INANITY_OBJECT_HPP___

#include "RefCounted.hpp"
#include <cstddef>

BEGIN_INANITY

/// Managed object class.
/** A simple object allocated in global managed heap. */
class Object : public RefCounted
{
protected:
	//*** RefCounted's method.
	void FreeAsNotReferenced();

public:
	static void* operator new(size_t size);
	static void operator delete(void* data);
};

//*** Macros to create managed object with debug information
#ifdef _DEBUG
#define INANITY_SIDENS2(x) #x
#define INANITY_SIDENS(x) INANITY_SIDENS2(x)
#define NEW(...) Inanity::ObjectSetAllocationInfo(new __VA_ARGS__, #__VA_ARGS__ "  " __FILE__ "(" INANITY_SIDENS(__LINE__) ")")
#define NEW_WITH_TAG(tag, ...) Inanity::ObjectSetAllocationInfo(new __VA_ARGS__, "[" tag "] " #__VA_ARGS__ "  " __FILE__ "(" INANITY_SIDENS(__LINE__) ")")
// defined in ManagedHeap.cpp
void ManagedHeapSetAllocationInfo(void*, const char* info);
template <typename T>
T* ObjectSetAllocationInfo(T* data, const char* info)
{
	ManagedHeapSetAllocationInfo(data, info);
	return data;
}
#else
#define NEW(...) new __VA_ARGS__
#define NEW_WITH_TAG(tag, ...) new __VA_ARGS__
#endif

END_INANITY

#endif
