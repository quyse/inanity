#ifndef ___INANITY_TYPED_POOL_HPP___
#define ___INANITY_TYPED_POOL_HPP___

#include "ChunkPool.hpp"

BEGIN_INANITY

/// Templated pool of instances of one type.
template <typename T>
class TypedPool : public ChunkPool
{
public:
	TypedPool() : ChunkPool(sizeof(T)) {}

	T* Allocate()
	{
		return (T*)ChunkPool::Allocate();
	}

	void Free(T* t)
	{
		ChunkPool::Free(t);
	}
};

END_INANITY

#endif
