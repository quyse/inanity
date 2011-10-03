#include "ManagedHeap.hpp"
#include <cstdlib>
#include <cstdio>

BEGIN_INANITY

ManagedHeap managedHeap;

END_INANITY

#ifdef ___INANITY_WINDOWS
#define DEBUG_PRINT(x) OutputDebugStringA(x)
#else
#define DEBUG_PRINT(x) printf("%s", (x))
#endif

#ifdef TRACE_HEAP
ManagedHeap::AllocationInfo::AllocationInfo(size_t number, size_t size) : number(number), size(size)
{
}
#endif

ManagedHeap::ManagedHeap()
#ifdef TRACE_HEAP
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

#ifdef TRACE_HEAP
	//выдать отчет по памяти
	DEBUG_PRINT("======= INANITY MANAGED HEAP REPORT =======\n");
	static char s[100];
	sprintf(s, "Allocations count: %zu\nAllocations size: %zu\n", allocationsCount, allAllocationsSize);
	DEBUG_PRINT(s);
	if(allocations.size())
	{
#ifdef ___INANITY_WINDOWS
		Beep(750, 300);
#endif
		DEBUG_PRINT("ATTENTION! SOME LEAKS DETECTED!\n");
		PrintAllocations();
	}
	else
		DEBUG_PRINT("NO LEAKS DETECTED\n");
	DEBUG_PRINT("======= END REPORT =======\n");
#endif
}

#ifdef TRACE_HEAP
void ManagedHeap::PrintAllocations()
{
	static char s[1024];
	for(std::map<void*, AllocationInfo>::const_iterator i = allocations.begin(); i != allocations.end(); ++i)
	{
		sprintf(s, "%p : #%zu, %zu bytes\n", i->first, i->second.number, i->second.size);
		DEBUG_PRINT(s);
		if(i->second.info)
		{
			sprintf(s, "  %s\n", i->second.info);
			DEBUG_PRINT(s);
		}
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

#ifdef TRACE_HEAP
	if(allocationsCount == tracedBlock)
		DebugBreak();
	allocations.insert(std::make_pair(data, AllocationInfo(allocationsCount++, size)));
	allAllocationsSize += size;
#endif
	return data;
}

void ManagedHeap::Free(void *data)
{
#ifdef TRACE_HEAP
	std::map<void*, AllocationInfo>::iterator i = allocations.find(data);
	if(i == allocations.end())
		DebugBreak();
	allocations.erase(i);
#endif

#ifdef ___INANITY_WINDOWS
	if(!HeapFree(heap, 0, data))
		ExitProcess(1);
#else
	free(data);
#endif
}

#ifdef TRACE_HEAP

void ManagedHeap::SetAllocationInfo(void* data, const char* info)
{
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
