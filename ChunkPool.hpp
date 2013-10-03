#ifndef ___INANITY_CHUNK_POOL_HPP___
#define ___INANITY_CHUNK_POOL_HPP___

#include "MemoryPool.hpp"

BEGIN_INANITY

/// Class which allocates and frees chunks - equal-sized pieces of memory.
/** Free chunks organized to single-linked list.
Memory of free chunk is used as a pointer to a next element in a list.
*/
class ChunkPool : public MemoryPool
{
private:
	/// Size of one chunk.
	size_t chunkSize;
	/// Number of chunks in a batch (for memory allocation).
	size_t chunksInBatchCount;
	/// Pointer to first free chunk.
	void* firstFreeChunk;

public:
	/// Create chunk pool.
	/** If chunksInBatchCount == 0, it'll be set to some default value. */
	ChunkPool(size_t chunkSize, size_t chunksInBatchCount = 0);

	void* Allocate();
	void Free(void* chunk);
};

END_INANITY

#endif
