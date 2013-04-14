#include "ManagedHeap.hpp"

BEGIN_INANITY

void* Object::operator new(size_t size)
{
	return managedHeap.Allocate(size);
}

void Object::operator delete(void* data)
{
	return managedHeap.Free(data);
}

END_INANITY
