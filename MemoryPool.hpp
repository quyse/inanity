#ifndef ___INANITY_MEMORY_POOL_HPP___
#define ___INANITY_MEMORY_POOL_HPP___

#include "Object.hpp"
#include <vector>

BEGIN_INANITY

/// Class which allocates and tracks chunks of memory.
/** Memory may only be allocated. All memory will be
freed at once when pool is destroyed. */
class MemoryPool : public Object
{
private:
	std::vector<char*> regions;

public:
	MemoryPool();
	~MemoryPool();

	void* Allocate(size_t size);
};

END_INANITY

#endif
