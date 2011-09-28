#ifndef ___INANITY_MANAGED_HEAP_HPP___
#define ___INANITY_MANAGED_HEAP_HPP___

/*
 * Файл, включающий класс управляемой кучи.
 */

#include "Object.hpp"
#ifdef ___INANITY_WINDOWS
#include <windows.h>
#endif
#include <vector>

#ifdef _DEBUG
//макрос для трассировки памяти
#define TRACE_HEAP
#endif

#ifdef TRACE_HEAP
#include <map>
#endif

BEGIN_INANITY

//класс управляемой кучи
class ManagedHeap
{
private:
	//информация для трассировки
#ifdef TRACE_HEAP

	size_t allocationsCount;
	size_t allAllocationsSize;
	struct AllocationInfo
	{
		size_t number;
		size_t size;
		const char* info;
		AllocationInfo(size_t number, size_t size);
	};
	std::map<void*, AllocationInfo> allocations;

#endif

	//максимальный размер кусочка, выделяемый быстрым способом
	static const size_t maxChunkSize = 1 << 10;
	//шаг между размерами кусочков
	static const size_t chunkSizeStep = 8;
	//количество аллокаторов для разных размеров
	static const size_t allocatorsCount = maxChunkSize / chunkSizeStep;

private:
#ifdef ___INANITY_WINDOWS
	//куча для медленного выделения памяти
	HANDLE heap;
#endif

#ifdef TRACE_HEAP
public:
	//значение указателя на выделяемую память, при котором будет выдаваться breakpoint
	size_t tracedBlock;

	//функция для указания дополнительной информации о кусочке памяти
	void SetAllocationInfo(void* data, const char* info);
#endif

public:
	ManagedHeap();
	~ManagedHeap();

	void* Allocate(size_t size);
	void Free(void* data);
#ifdef TRACE_HEAP
	void PrintAllocations();
#endif
};
//глобальная управляемая куча
extern ManagedHeap managedHeap;

END_INANITY

#endif
