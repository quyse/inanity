#include "ManagedHeap.hpp"
#ifdef ___INANITY_TRACE_HEAP
#include "CriticalCode.hpp"
#include "Log.hpp"
#endif
#include <map>
#include <sstream>
#include <algorithm>

BEGIN_INANITY

#if !defined(___INANITY_PLATFORM_WINDOWS)
#define DebugBreak() asm("int3")
#endif

#ifdef ___INANITY_TRACE_PTR
/// Выключить трассировку указателей?
/** Она очень замедляет работу в debug. */
static const bool disableTracePtr = true;
#endif

ManagedHeap managedHeap;

#ifdef ___INANITY_TRACE_HEAP
ManagedHeap::AllocationInfo::AllocationInfo(size_t number, size_t size) : number(number), size(size)
{
}
#endif

ManagedHeap::ManagedHeap()
#ifdef ___INANITY_TRACE_HEAP
: totalAllocationsCount(0), totalAllocationsSize(0)
#endif
{
#ifdef ___INANITY_PLATFORM_WINDOWS

	// создать кучу (растущую, с нулевым начальным размером)
	heap = HeapCreate(0, 0, 0);
	if(!heap)
		exit(1);
	// сделать кучу low-fragmentation
	ULONG heapFragValue = 2;
	// обработка ошибок не производится, чтобы работало на не-XP и не-Vista
	HeapSetInformation(heap, HeapCompatibilityInformation, &heapFragValue, sizeof(heapFragValue));

#endif // ___INANITY_PLATFORM_WINDOWS
}

ManagedHeap::~ManagedHeap()
{
#ifdef ___INANITY_PLATFORM_WINDOWS

	// free all heap's memory
	HeapDestroy(heap);

#endif

#ifdef ___INANITY_TRACE_HEAP

	// create memory report
	Log::Message("======= INANITY MANAGED HEAP REPORT =======");
	Log::Message("allocations: ", totalAllocationsCount, ", total size: ", totalAllocationsSize);
	if(allocations.size())
	{
		std::ostringstream stream;
		PrintAllocations(stream);
		Log::Warning("ATTENTION! SOME LEAKS DETECTED!\n", stream.str());
	}
	else
		Log::Message("NO LEAKS DETECTED");

#ifdef ___INANITY_TRACE_PTR

	if(!disableTracePtr)
	{
		std::ostringstream stream;
		PrintPtrs(stream);
		Log::Message("======= TRACE PTR REPORT =======\n", stream.str());
	}

#endif

	Log::Message("======= END REPORT =======");

#endif
}

void* ManagedHeap::Allocate(size_t size)
{
#ifdef ___INANITY_PLATFORM_WINDOWS

	void* data = HeapAlloc(heap, 0, size);
	if(!data)
		ExitProcess(1);

#else

	void* data = malloc(size);

#endif

#ifdef ___INANITY_TRACE_HEAP

	{
		CriticalCode code(criticalSection);

		allocations.insert(std::make_pair(data, AllocationInfo(totalAllocationsCount++, size)));
		totalAllocationsSize += size;
	}

#endif
	return data;
}

void ManagedHeap::Free(void *data)
{
	// освободить память
#ifdef ___INANITY_PLATFORM_WINDOWS
	if(!HeapFree(heap, 0, data))
		ExitProcess(1);
#else
	free(data);
#endif

	// отметить, что память удалена
#ifdef ___INANITY_TRACE_HEAP
	{
		CriticalCode code(criticalSection);

		Allocations::iterator i = allocations.find(data);
		if(i == allocations.end())
			DebugBreak();
		allocations.erase(i);
	}
#endif
}

#ifdef ___INANITY_TRACE_HEAP

void ManagedHeap::PrintAllocations(std::ostream& stream)
{
	CriticalCode code(criticalSection);

	for(Allocations::const_iterator i = allocations.begin(); i != allocations.end(); ++i)
	{
		stream << i->first << " : #" << i->second.number << ", " << i->second.size << " bytes\n";
		if(i->second.info)
			stream << "  " << i->second.info << "\n";
	}
}

void ManagedHeap::SetAllocationInfo(void* data, const char* info)
{
	CriticalCode code(criticalSection);

	Allocations::iterator i = allocations.find(data);
	if(i != allocations.end())
		i->second.info = info;
}

void ManagedHeapSetAllocationInfo(void* data, const char* info)
{
	managedHeap.SetAllocationInfo(data, info);
}

#ifdef ___INANITY_TRACE_PTR

class ManagedHeap::PtrTracer
{
	struct Object
	{
		void* data;
		size_t size;
		const char* info;

		Object(void* data, size_t size, const char* info) : data(data), size(size), info(info) {}
	};

	struct Sorter
	{
		bool operator()(const Object& a, const Object& b) const
		{
			return a.data < b.data;
		}

		bool operator()(void* a, const Object& b) const
		{
			return a < b.data;
		}

		bool operator()(const Object& a, void* b) const
		{
			return a.data < b;
		}
	} sorter;

	typedef std::vector<Object> Objects;
	Objects objects;
	typedef std::multimap<Objects::const_iterator, Objects::const_iterator> Links;
	Links links;

	std::vector<bool> levels;
	std::vector<Objects::const_iterator> levelObjects;

public:
	PtrTracer(const ManagedHeap& heap)
	{
		// сформировать список объектов
		objects.reserve(heap.allocations.size());
		for(Allocations::const_iterator i = heap.allocations.begin(); i != heap.allocations.end(); ++i)
			objects.push_back(Object(i->first, i->second.size, i->second.info));
		std::sort(objects.begin(), objects.end(), sorter);

		// сформировать карту ссылок
		for(Ptrs::const_iterator i = heap.ptrs.begin(); i != heap.ptrs.end(); ++i)
		{
			// найти объект, в котором содержится указатель
			Objects::const_iterator j = std::upper_bound(objects.begin(), objects.end(), i->first, sorter);
			if(j > objects.begin())
			{
				--j;
				if((size_t)((char*)i->first - (char*)j->data) < j->size)
					// да, указатель содержится в этом объекте
					// получить объект, на который указывает указатель, и добавить ссылку
					links.insert(std::make_pair(j, std::lower_bound(objects.begin(), objects.end(), i->second, sorter)));
			}
		}
	}

	void Print(std::ostream& stream, Objects::const_iterator object, bool last = false)
	{
		for(size_t i = 0; i < levels.size(); ++i)
			if(i == levels.size() - 1)
				stream << "|_";
			else if(levels[i])
				stream << "| ";
			else
				stream << "  ";

		bool cyclicReference = std::find(levelObjects.begin(), levelObjects.end(), object) != levelObjects.end();
		if(cyclicReference)
			stream << "CYCLIC ";

		stream << object->data << ", " << object->info << "\n";

		if(cyclicReference)
			return;

		if(last)
			levels[levels.size() - 1] = false;

		levelObjects.push_back(object);

		Links::const_iterator link = links.lower_bound(object);
		Links::const_iterator endLink = links.upper_bound(object);
		if(link != endLink)
		{
			levels.push_back(true);
			Links::const_iterator nextLink;
			for(; link != links.end() && link->first == object; link = nextLink)
			{
				nextLink = link;
				++nextLink;
				Print(stream, link->second, nextLink == links.end() || nextLink->first != object);
			}
			levels.pop_back();
		}

		levelObjects.pop_back();
	}

	void Print(std::ostream& stream)
	{
		std::vector<Objects::const_iterator> referencedObjects;
		referencedObjects.reserve(links.size());
		for(Links::const_iterator i = links.begin(); i != links.end(); ++i)
			referencedObjects.push_back(i->second);
		std::sort(referencedObjects.begin(), referencedObjects.end());
		referencedObjects.resize(std::unique(referencedObjects.begin(), referencedObjects.end()) - referencedObjects.begin());

		// пока просто вывести
		for(Objects::const_iterator object = objects.begin(); object != objects.end(); ++object)
		{
			if(std::binary_search(referencedObjects.begin(), referencedObjects.end(), object))
				continue;
			Print(stream, object);
		}
	}
};

void ManagedHeap::PrintPtrs(std::ostream& stream)
{
	CriticalCode code(criticalSection);

	PtrTracer tracer(*this);

	tracer.Print(stream);
}

void ManagedHeap::TracePtr(void* ptr, void* object)
{
	if(disableTracePtr)
		return;

	CriticalCode code(criticalSection);

	if(object)
		ptrs[ptr] = object;
	else
		ptrs.erase(ptr);
}

void ManagedHeapTracePtr(void* ptr, void* object)
{
	managedHeap.TracePtr(ptr, object);
}

#endif

#endif

END_INANITY
