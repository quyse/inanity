#include "SafeObject.hpp"
#include "ManagedHeap.hpp"

BEGIN_INANITY

void* SafeObject::operator new(size_t size)
{
	return managedHeap.Allocate(size);
}

void SafeObject::operator delete(void* data)
{
	return managedHeap.Free(data);
}

END_INANITY
