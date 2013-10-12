#ifndef ___INANITY_MANAGED_HEAP_HPP___
#define ___INANITY_MANAGED_HEAP_HPP___

/*
 * Файл, включающий класс управляемой кучи.
 */

#include "Object.hpp"
#ifdef ___INANITY_PLATFORM_WINDOWS
#include "platform/windows.hpp"
#endif
#include <vector>
#include <iostream>

#ifdef ___INANITY_TRACE_HEAP
#include "CriticalSection.hpp"
#include <unordered_map>
#endif

BEGIN_INANITY

//класс управляемой кучи
class ManagedHeap
{
private:
	/// Информация для трассировки кучи.
#ifdef ___INANITY_TRACE_HEAP

	CriticalSection criticalSection;
	/// Общее количество создаваний объектов (не уменьшается).
	size_t totalAllocationsCount;
	/// Общее количество выделенной памяти на объекты (не уменьшается).
	size_t totalAllocationsSize;
	struct AllocationInfo
	{
		size_t number;
		size_t size;
		const char* info;
		AllocationInfo(size_t number, size_t size);
	};
	typedef std::unordered_map<void*, AllocationInfo> Allocations;
	Allocations allocations;

#ifdef ___INANITY_TRACE_PTR

	/// Карта ссылок указателей на объекты.
	typedef std::unordered_map<void*, void*> Ptrs;
	Ptrs ptrs;

	class PtrTracer;

#endif

#endif

private:
#ifdef ___INANITY_PLATFORM_WINDOWS
	//куча для медленного выделения памяти
	HANDLE heap;
#endif

public:
	ManagedHeap();
	~ManagedHeap();

	void* Allocate(size_t size);
	void Free(void* data);

//*** Открытые методы для трассировки.
#ifdef ___INANITY_TRACE_HEAP

	/// Распечатать список всей выделенной памяти.
	void PrintAllocations(std::ostream& stream);
	/// Указать дополнительную информацию о кусочке памяти.
	void SetAllocationInfo(void* data, const char* info);

#ifdef ___INANITY_TRACE_PTR

	/// Распечатать список всех указателей.
	void PrintPtrs(std::ostream& stream);
	/// Сообщить, что данный указатель теперь указывает на данный объект.
	void TracePtr(void* ptr, void* object);

#endif

#endif
};
//глобальная управляемая куча
extern ManagedHeap managedHeap;

END_INANITY

#endif
