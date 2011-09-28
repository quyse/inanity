#include "ManagedHeap.hpp"

void* Object::operator new(size_t size)
{
	return managedHeap.Allocate(size);
}

void Object::operator delete(void* data)
{
	return managedHeap.Free(data);
}
