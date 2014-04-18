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

	template <typename... Args>
	T* New(Args... args)
	{
		void* chunk = ChunkPool::Allocate();
		return new (chunk) T(args...);
	}

	void Delete(T* t)
	{
		// hold a pointer to itself (in case ~T releases last reference)
		ptr<TypedPool> This = this;

		t->~T();
		ChunkPool::Free(t);
	}
};

END_INANITY

#endif
