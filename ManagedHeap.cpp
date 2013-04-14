#include "ManagedHeap.hpp"
#ifdef ___INANITY_TRACE_HEAP
#include "CriticalCode.hpp"
#endif
#include <iostream>

BEGIN_INANITY

ManagedHeap managedHeap;

END_INANITY

#ifdef ___INANITY_TRACE_HEAP
ManagedHeap::AllocationInfo::AllocationInfo(size_t number, size_t size) : number(number), size(size)
{
}
#endif

ManagedHeap::ManagedHeap()
#ifdef ___INANITY_TRACE_HEAP
: allocationsCount(0), allAllocationsSize(0), tracedBlock(-1)
#endif
{
#ifdef ___INANITY_WINDOWS
	//создать кучу (растущую, с нулевым начальным размером)
	heap = HeapCreate(0, 0, 0);
	if(!heap)
		exit(1);
	//сделать кучу low-fragmentation
	ULONG heapFragValue = 2;
	//обработка ошибок не производится, чтобы работало на не-XP и не-Vista
	//хотя весь движок работает под DirectX 10 => Vista :)
	HeapSetInformation(heap, HeapCompatibilityInformation, &heapFragValue, sizeof(heapFragValue));
#endif // ___INANITY_WINDOWS
}

ManagedHeap::~ManagedHeap()
{
#ifdef ___INANITY_WINDOWS
	//удалить кучу
	HeapDestroy(heap);
#endif

#ifdef ___INANITY_TRACE_HEAP
	//выдать отчет по памяти
	std::cout << "======= INANITY MANAGED HEAP REPORT =======\n";
	std::cout << "Allocations count: " << allocationsCount <<  "\nAllocations size: " << allAllocationsSize << "\n";
	if(allocations.size())
	{
#ifdef ___INANITY_WINDOWS
		Beep(750, 300);
#endif
		std::cout << "ATTENTION! SOME LEAKS DETECTED!\n";
		PrintAllocations();
	}
	else
		std::cout << "NO LEAKS DETECTED\n";
	std::cout << "======= END REPORT =======\n";
#endif
}

#ifdef ___INANITY_TRACE_HEAP
void ManagedHeap::PrintAllocations()
{
	for(std::map<void*, AllocationInfo>::const_iterator i = allocations.begin(); i != allocations.end(); ++i)
	{
		std::cout << i->first << " : #" << i->second.number << ", " << i->second.size << " bytes\n";
		if(i->second.info)
			std::cout << "  " << i->second.info << "\n";
	}
}
#endif

void* ManagedHeap::Allocate(size_t size)
{
#ifdef ___INANITY_WINDOWS
	void* data = HeapAlloc(heap, 0, size);
	if(!data)
		ExitProcess(1);
#else
	void* data = malloc(size);
#endif

#ifdef ___INANITY_TRACE_HEAP
	{
		CriticalCode code(criticalSection);
		if(allocationsCount == tracedBlock)
			DebugBreak();
		allocations.insert(std::make_pair(data, AllocationInfo(allocationsCount++, size)));
		allAllocationsSize += size;
	}
#endif
	return data;
}

void ManagedHeap::Free(void *data)
{
#ifdef ___INANITY_TRACE_HEAP
	{
		CriticalCode code(criticalSection);
		std::map<void*, AllocationInfo>::iterator i = allocations.find(data);
		if(i == allocations.end())
			DebugBreak();
		allocations.erase(i);
	}
#endif

#ifdef ___INANITY_WINDOWS
	if(!HeapFree(heap, 0, data))
		ExitProcess(1);
#else
	free(data);
#endif
}

#ifdef ___INANITY_TRACE_HEAP

void ManagedHeap::SetAllocationInfo(void* data, const char* info)
{
	CriticalCode code(criticalSection);
	std::map<void*, AllocationInfo>::iterator i = allocations.find(data);
	if(i != allocations.end())
		i->second.info = info;
}

BEGIN_INANITY

void ManagedHeapSetAllocationInfo(void* data, const char* info)
{
	managedHeap.SetAllocationInfo(data, info);
}

END_INANITY

#endif
