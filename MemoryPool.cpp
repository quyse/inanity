#include "MemoryPool.hpp"

BEGIN_INANITY

MemoryPool::MemoryPool() {}

MemoryPool::~MemoryPool()
{
	for(size_t i = 0; i < regions.size(); ++i)
		delete [] regions[i];
}

void* MemoryPool::Allocate(size_t size)
{
	char* region = new char[size];
	regions.push_back(region);
	return region;
}

END_INANITY
