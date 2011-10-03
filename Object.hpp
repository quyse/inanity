#ifndef ___INANITY_OBJECT_HPP___
#define ___INANITY_OBJECT_HPP___

#include "ptr.hpp"
#include <cstddef>

BEGIN_INANITY

/// Класс управляемого объекта. Базовый для всех классов, объекты которых создаются в управляемой куче.
class Object
{
private:
	int referencesCount;

public:
	inline Object() : referencesCount(0)
	{
	}

	virtual ~Object()
	{
	}

	inline void Reference()
	{
		referencesCount++;
	}

	inline void Dereference()
	{
		if(!--referencesCount)
			delete this;
	}

	inline unsigned GetReferencesCount() const
	{
		return referencesCount;
	}

	static void* operator new(size_t size);
	static void operator delete(void* data);
};

//макросы для выделения памяти с указанием информации о выделяемом кусочке
#ifdef _DEBUG
#define INANITY_SIDENS2(x) #x
#define INANITY_SIDENS(x) INANITY_SIDENS2(x)
#define NEW(o) ObjectSetAllocationInfo(new o, __FILE__ "(" INANITY_SIDENS(__LINE__) "): " #o)
#define NEW_WITH_TAG(tag, o) ObjectSetAllocationInfo(new o, __FILE__ "(" INANITY_SIDENS(__LINE__) "), tag: [" tag "]: " #o)
//функция определена в ManagedHeap.cpp
void ManagedHeapSetAllocationInfo(void*, const char* info);
template <typename T>
T* ObjectSetAllocationInfo(T* data, const char* info)
{
	ManagedHeapSetAllocationInfo(data, info);
	return data;
}
#else
#define NEW(o) new o
#define NEW_WITH_TAG(tag, o) new o
#endif

END_INANITY

#endif
